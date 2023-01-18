
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

namespace oct::cave::v0
{


class ExceptionResult : public core::v3::Exception
{
public:
	
public:
	ExceptionResult();
	ExceptionResult(const ExceptionResult&);

	ExceptionResult(const char* message);
	ExceptionResult(const char* message,const char* filename, unsigned int line);
	virtual ~ExceptionResult();

private:
};

class ExceptionQuery : public core::v3::Exception
{
public:
	
public:
	ExceptionQuery();
	ExceptionQuery(const ExceptionQuery&);

	ExceptionQuery(const char* message);
	ExceptionQuery(const char* message,const char* filename, unsigned int line);
	virtual ~ExceptionQuery();

private:
};

typedef void* Handle;
typedef unsigned long index;


class DataSource
{
public:
	DataSource() = default;
	DataSource(const std::string& database);
	
	const std::string& get_database()const;
	
protected:
	std::string database;
};



template<class S>
concept Row = requires(S s)
{
	s = (const char**)0;//operator de asignacio para arreglo de c strings
};



template<typename D> class Result
{
public:
	Result() : result(NULL)
	{
		//std::cout << "Result()\n";
	}
	Result(Result<D>&& r) 
	{
		result = r.result;
		r.result = NULL;
		//std::cout << "Result(Result<D>&& " << result << ")\n";
	}
	Result(const Result<D>&) 
	{
		throw ExceptionResult("No puede ser copiado este objeto",__FILE__,__LINE__);
	}
	Result(Handle&& h)
	{
		result = h;
		//std::cout << "Result(Handle&& " << result << ")\n";
	}
	~Result();

	void operator =(Result&& r)
	{
		if(result) throw ExceptionResult("El objeto deve estar vacio para realizar esta operacion.",__FILE__,__LINE__);

		result = r.result;
		//std::cout << "Result& operator =(Result&&  " << result << ")\n";
		r.result = NULL;
	}
	const Result<D>& operator =(const Result<D>&)
	{
		throw ExceptionResult("No puede ser copiado este objeto",__FILE__,__LINE__);
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

typedef std::vector<std::string> fields;

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
	explicit operator Handle()
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
		//std::cout << srtsql << "\n";

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
		//std::cout << srtsql << "\n";

		return execute(srtsql);
	}
	Result<Data> select(const fields& list,const std::string& table,const std::string& where)
	{
		std::string srtsql = "SELECT ";
		if(list.size() > 1)
		{
			for(size_t i = 0; i < list.size() - 2; i++)
			{
				srtsql += list[i];
			}
		}
		else
		{
			srtsql += list[0];
		}
		
		srtsql += " FROM ";
		srtsql += table;
		srtsql += " WHERE ";
		srtsql += where;
		srtsql += ";";
		//std::cout << srtsql << "\n";

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