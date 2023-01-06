
#include <glibmm/i18n.h>
#include <string>
#include <iostream>

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
typedef void* Handle;
struct Data
{
	virtual const char* get_name() const = 0;
	virtual const char* get_label() const = 0;

};

struct DataMaria : public Data
{
	DataMaria(const std::string& host,const std::string& user,const std::string& password);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,unsigned int port);	
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port,const std::string& socket);
	DataMaria(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port,const std::string& socket,unsigned long flags);
	~DataMaria();
		
	virtual const char* get_name() const;
	virtual const char* get_label() const;
	const std::string& get_host()const;
	const std::string& get_user()const;
	const std::string& get_password()const;
	const std::string& get_database()const;
	const std::string& get_socket()const;
	unsigned int get_port()const;
	unsigned long get_flags()const;


	//data members
	std::string host,user,password,database,socket;
	unsigned int port;
	unsigned long flags;	
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
		result = r.result;
		std::cout << "Result& operator =(Result&&  " << result << ")\n";
		r.result = NULL;
		return *this;
	}
	bool is_stored()const
	{
		return (result ? true : false);
	}
	
private:
	Handle result;
};

template<typename Data> class Connection
{
public:
	Connection(const Data& data);	
	~Connection();

	bool is_connected()const
	{
		return connected;
	}
	operator Handle()
	{
		return connection;
	}

	Result<Data> execute(const std::string&);

protected:

private:
	void* connection;
	bool connected;

};


}