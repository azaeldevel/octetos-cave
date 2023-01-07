
#include "client.hh"

namespace oct::cave
{

	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd) : host(h),user(u),password(pwd)
	{
	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d)  : host(h),user(u),password(pwd),database(d)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,unsigned int p) : host(h),user(u),password(pwd),port(p)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p) : host(h),user(u),password(pwd),database(d),port(p)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p,const std::string& s) : host(h),user(u),password(pwd),database(d),port(p),socket(s)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p,const std::string& s,unsigned long f) : host(h),user(u),password(pwd),database(d),port(p),socket(s),flags(f)
	{
	
	}
	DataMaria::~DataMaria()
	{
	}
	
	
	const char* DataMaria::get_name() const
	{
		return "maria";
	}
	const char* DataMaria::get_label() const
	{
		return "Maria";
	}

	

	const std::string& DataMaria::get_host()const
	{
		return host;
	}
	const std::string& DataMaria::get_user()const
	{
		return user;
	}
	const std::string& DataMaria::get_password()const
	{
		return password;
	}
	const std::string& DataMaria::get_database()const
	{
		return database;
	}
	const std::string& DataMaria::get_socket()const
	{
		return socket;
	}
	unsigned int DataMaria::get_port()const
	{
		return port;
	}
	unsigned long DataMaria::get_flags()const
	{
		return flags;
	}
	
	
	
	
	
	
	
	
	
	
	
	template<> Connection<DataMaria>::Connection(const DataMaria& data): connection((void*)mysql_init(NULL)),connected(false)
	{
		MYSQL* con = mysql_real_connect(
						 (MYSQL*)connection, 
						 data.get_host().empty() ? NULL : data.get_host().c_str(), 
						 data.get_user().empty() ? NULL : data.get_user().c_str(), 
						 data.get_password().empty() ? NULL : data.get_password().c_str(), 
						 data.get_database().empty() ? NULL : data.get_database().c_str(), 
						 data.get_port(), 
						 data.get_socket().empty() ? NULL : data.get_socket().c_str(), 
						 data.get_flags()
		                             	);
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
	template<> Connection<DataMaria>::~Connection()
	{
		if(connection)
		{
			mysql_close((MYSQL*)connection);
			connection = NULL;
		}		
	}

	template<> Result<DataMaria> Connection<DataMaria>::execute(const std::string& str)
	{
		if (mysql_query((MYSQL*)connection, str.c_str()) != 0) 
		{
			//std::cout << "Erro : " << mysql_error((MYSQL*)connection) << "\n";
			return Result<DataMaria>();
		}

		return Result<DataMaria>(mysql_store_result((MYSQL*)connection));
	}




}