
#ifndef OCTETOS_CAVE_CLIENT_HH
#define OCTETOS_CAVE_CLIENT_HH

#ifdef OCTEOTOS_CAVE_ENABLE_DEV
	#include <iostream>
#endif

#include <glibmm/i18n.h>
#include <string>
#include <vector>
#include <list>
#include <octetos/core/Exception-v3.hh>


#include "config.h"
#if defined LINUX_ARCH
	#include <mysql/mysql.h>
#elif defined LINUX_GENTOO
	#include <mariadb/mysql.h>
#elif defined LINUX_DEBIAN
	#include <mariadb/mysql.h>
#elif defined WINDOWS_MINGW && defined BUILDING_DLL
    #include <mariadb/mysql.h>
#elif MSYS2
    #include <mariadb/mysql.h>
#else
	#error "Plataforma desconocida."
#endif

namespace oct::cave
{
//class DataMaria;
//template<> Connection<DataMaria>;


class Exception : public oct::core::v3::Exception
{
public:
	enum ErrosCodes
	{
		NoErros,
		NotYet,
		IlegalOperation,
		NotCopiable,
		NotCopiableResult,
		NotVoidObject,
		DB_ERROR,
		DB_ERROR_Result,
		DB_ERROR_Result_Unknow,
	};
public:
	Exception();
	Exception(const Exception&);
	Exception(Exception&&);

	Exception(unsigned int code);
	Exception(unsigned int code,const char* filename, unsigned int line);

	Exception(unsigned int code,const char* message);
	Exception(unsigned int code,const char* message,const char* filename, unsigned int line);

	Exception(const std::string& message);
	Exception(const std::string& message,const char* filename, unsigned int line);

	Exception(unsigned int code,const std::string& message);
	Exception(unsigned int code,const std::string& message,const char* filename, unsigned int line);
	virtual ~Exception();

	virtual const char* what() const throw ();

private:
};

class ExceptionMaria : public Exception
{
public:
public:
	ExceptionMaria();
	ExceptionMaria(const ExceptionMaria&);
	ExceptionMaria(ExceptionMaria&&);

	ExceptionMaria(unsigned int code,const char* state,const char* filename, unsigned int line);
	virtual ~ExceptionMaria();

	virtual const char* what() const throw ();

private:
	const char* state;
};

typedef void* Handle;
typedef unsigned long index;

class DataMaria
{
public:
	DataMaria(const std::string& host,const std::string& user,const std::string& password);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,unsigned int port);	
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port,const std::string& socket);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port,const std::string& socket,unsigned long flags);
	~DataMaria();
		
	const std::string& get_host()const;
	const std::string& get_user()const;
	const std::string& get_password()const;
	const std::string& get_database()const;
	const std::string& get_socket()const;
	unsigned int get_port()const;
	unsigned long get_flags()const;
	
private:
	std::string host,user,password,database,socket;
	unsigned int port;
	unsigned long flags;	
};


template<class S>
concept Row = requires(S s)
{
	s = (const char**)0;//operator de asignacio para arreglo de c strings
};


template<class S>
concept RowMaria = requires(S s)
{
	s = (char**)0;//operator de asignacio para arreglo de c strings
};

template<typename D> class Result
{
public:
	Result() : result(NULL)
	{
		std::cout << "Result()\n";
	}
	Result(Result<D>&& r) 
	{
		result = r.result;
		r.result = NULL;
		std::cout << "Result(Result<D>&& " << result << ")\n";
	}
	Result(const Result<D>&) 
	{
		throw Exception(Exception::NotCopiableResult,__FILE__,__LINE__);
	}
	Result(Handle&& h)
	{
		result = h;
		std::cout << "Result(Handle&& " << result << ")\n";
	}
	~Result();

	operator Handle()
	{
		return result;
	}
	Result& operator =(Result&& r)
	{
		if(result) throw Exception(Exception::NotVoidObject,__FILE__,__LINE__);

		result = r.result;
		std::cout << "Result& operator =(Result&&  " << result << ")\n";
		r.result = NULL;
		return *this;
	}
	const Result<D>& operator =(const Result<D>&)
	{
		throw Exception(Exception::NotCopiableResult,__FILE__,__LINE__);
	}
	bool is_stored() const
	{
		return (result ? true : false);
	}
	void close();

	unsigned long number_rows()const;
		
	template<Row S> void store(std::vector<S>& v);
	template<Row S> void store(std::list<S>& v);

private:
	Handle result;
};


template<> class Result<DataMaria>
{
public:
	Result() : result(NULL)
	{
		std::cout << "Result()\n";
	}
	Result(Result<DataMaria>&& r) 
	{
		result = r.result;
		r.result = NULL;
		std::cout << "Result(Result<D>&& " << result << ")\n";
	}
	Result(Handle&& h)
	{		
		result = h;
		std::cout << "Result(Handle&& " << result << ")\n";
	}
	~Result()
	{
		if(result)
		{
			mysql_free_result((MYSQL_RES*)result);
			result = NULL;
		}
	}

	operator Handle()
	{
		return result;
	}
	Result& operator =(Result&& r)
	{
		std::cout << "Result& operator =(Result&&  " << r.result << ")\n";
		result = r.result;
		r.result = NULL;
		return *this;
	}
	bool is_stored() const
	{
		return (result ? true : false);
	}

	unsigned long number_rows()const
	{
		if(result) return mysql_num_rows((MYSQL_RES*)result);
		
		return 0;
	}
	void close()
	{
		if(result)
		{
			mysql_free_result((MYSQL_RES*)result);
			result = NULL;
		}
	}
	
	template<RowMaria R> void store(std::vector<R>& v)
	{
#ifdef OCTEOTOS_CAVE_ENABLE_DEV
		//std::cout << "template<typename S> void store(std::vector<S>& v)\n";
#endif
		v.resize(number_rows());
		char** row;
		for(index i = 0; i < number_rows(); i++)
		{
			row = mysql_fetch_row((MYSQL_RES*)result);
			if(row)
			{
				v.at(i) = row;
			}
			else
			{
				;//error
			}
		}	
	}
	
private:
	Handle result;
};

template<typename Data> class Connection
{
public:
	Connection();
	Connection(const Data& data, bool autocommit);	
	~Connection();

	bool is_connected()const
	{
		return connected;
	}
	bool is_autocommit()const
	{
		return autocommit;
	}
	operator Handle()
	{
		return connection;
	}

	Result<Data> execute(const std::string&);
	Result<Data> select(const std::string& fields,const std::string& table)
	{
		std::string srtsql = "SELECT ";
		srtsql += fields;
		srtsql += " FROM ";
		srtsql += table;
		srtsql += ";";
		std::cout << srtsql << "\n";

		return execute(srtsql);
	}
	Result<Data> select(const std::string& fields,const std::string& table,const std::string& where)
	{
		std::string srtsql = "SELECT ";
		srtsql += fields;
		srtsql += " FROM ";
		srtsql += table;
		srtsql += " WHERE ";
		srtsql += where;
		srtsql += ";";
		std::cout << srtsql << "\n";

		return execute(srtsql);
	}
	
	bool begin();
	bool commit();
	bool rollback();

	bool connect(const Data&, bool autocommit);
	void close();
	bool ping();

protected:

private:
	bool connected;
	void* connection;
	bool autocommit;
};


}

#endif