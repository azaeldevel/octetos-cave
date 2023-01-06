
#include <glibmm/i18n.h>
#include <string>

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

template<typename Data>
class Connection
{
public:
	Connection(const Data& data) : connection((void*)mysql_init(NULL)),connected(false)
	{
		MYSQL* con = mysql_real_connect((MYSQL*)connection, data.host.c_str(), data.user.c_str(), data.password.c_str(), NULL, 0, NULL, 0);
		if(con)
		{
			connected = true;
		}
		else
		{
			mysql_close(con);
			connection = NULL;
		}		
	}
	
	~Connection()
	{
		if(connection)
		{
			mysql_close((MYSQL*)connection);
			connection = NULL;
		}		
	}

	bool is_connected()const
	{
		return connected;
	}

protected:

private:
	void* connection;
	bool connected;

};


}