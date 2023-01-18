
#include "maria.hh"

namespace oct::cave::v0
{



	DataSource::DataSource(const std::string& d) : database(d)
	{
	}
	const std::string& DataSource::get_database()const
	{
		return database;
	}

	

	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd)
	{
	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d)  : host(h),user(u),password(pwd),DataSource(d),port(3306)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,unsigned int p) : host(h),user(u),password(pwd),port(p)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p) : host(h),user(u),password(pwd),DataSource(d),port(p)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p,const std::string& s) : host(h),user(u),password(pwd),DataSource(d),port(p),socket(s)
	{

	}
	DataMaria::DataMaria(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p,const std::string& s,unsigned long f) : host(h),user(u),password(pwd),DataSource(d),port(p),socket(s),flags(f)
	{
	
	}
	DataMaria::~DataMaria()
	{
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
	
	
		
	
	
	template<> bool Connection<DataMaria>::connect(const DataMaria& data, bool a)
	{
		//if(connection) return false;//ya esta conectada
		//if(connected) return false;//ya esta conectada		
		MYSQL* con = mysql_real_connect(
						 reinterpret_cast<MYSQL*>(connection), 
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
			connected = false;
			connection = NULL;
		}

		if(mysql_autocommit(con,a) == 0) autocommit = a;

		return connected;
	}
	template<> Connection<DataMaria>::Connection(const DataMaria& d, bool a): connection((void*)mysql_init(NULL)),connected(false), autocommit(a)
	{
		connect(d,a);
	}
	template<> Connection<DataMaria>::~Connection()
	{
		if(connection)
		{
			mysql_close(reinterpret_cast<MYSQL*>(connection));
			connection = NULL;
		}		
	}

	template<> Result<DataMaria> Connection<DataMaria>::execute(const std::string& str)
	{
		int ret_query = mysql_query(reinterpret_cast<MYSQL*>(connection), str.c_str());
		
		if (ret_query == -1 and mysql_errno(reinterpret_cast<MYSQL*>(connection)) == 2000) 
		{
			throw ExceptionQuery("Se genero error desconocido como respuesta, este error podria deverse a un bug",__FILE__,__LINE__);
		}
		else if (ret_query == 0) 
		{
			return Result<DataMaria>(mysql_store_result(reinterpret_cast<MYSQL*>(connection)));
		}
		else
		{
			//std::cout << "Error Maria : " << mysql_errno(reinterpret_cast<MYSQL*>(connection)) << "  " << mysql_error(reinterpret_cast<MYSQL*>(connection)) << "\n";
			//std::cout << "ret_query : " << ret_query << "\n";
			//std::cout << "SQL String : " << str << "\n";
			//return Result<DataMaria>();
			throw ExceptionQuery("Fallo la ejecucion de la consulta",__FILE__,__LINE__);
		}		
	}


	
	template<> bool Connection<DataMaria>::commit()
	{
		if(connection) if(mysql_commit(reinterpret_cast<MYSQL*>(connection)) == 0) return true;

		return false;
	}
	template<> bool Connection<DataMaria>::rollback()
	{
		if(connection) if(mysql_rollback(reinterpret_cast<MYSQL*>(connection)) == 0) return true;

		return false;
	}


	template<> void Connection<DataMaria>::close()
	{
		if(connection)
		{
			mysql_close(reinterpret_cast<MYSQL*>(connection));
			connection = NULL;
		}
	}
	template<> bool Connection<DataMaria>::ping()
	{
		if(not connected) return false;
		if(connection)
		{
			if(mysql_ping(reinterpret_cast<MYSQL*>(connection))) return false;
			return true;
		}

		return false;
	}
}