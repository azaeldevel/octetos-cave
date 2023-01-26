

/*
 * Copyright (C) 2022 Azael R. <azael.devel@gmail.com>
 *
 * octetos-cave is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * octetos-cave is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "mmsql.hh"


namespace oct::cave::v0
{


	ExceptionSQL::ExceptionSQL(Handle h) : handle(h), core::v3::Exception(Code(mysql_errno(reinterpret_cast<MYSQL*>(h))))
	{
	}
	ExceptionSQL::ExceptionSQL(Handle h, const char* f, unsigned int l) : handle(h), core::v3::Exception(Code(mysql_errno(reinterpret_cast<MYSQL*>(h))),f,l)
	{
	}


	const char* ExceptionSQL::what() const noexcept
	{
		return mysql_error(reinterpret_cast<MYSQL*>(handle));
	}

	

	DataMMSQL::DataMMSQL(const std::string& h,const std::string& u,const std::string& pwd) : port(3306),flags(0)
	{
	}
	DataMMSQL::DataMMSQL(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d) : host(h),user(u),password(pwd),DataSource(d),port(3306),flags(0)
	{
	}
	DataMMSQL::DataMMSQL(const std::string& h,const std::string& u,const std::string& pwd,unsigned int p) : host(h),user(u),password(pwd),port(p),flags(0)
	{
	}
	DataMMSQL::DataMMSQL(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p) : host(h),user(u),password(pwd),DataSource(d),port(p),flags(0)
	{
	}
	DataMMSQL::DataMMSQL(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p,const std::string& s) : host(h),user(u),password(pwd),DataSource(d),port(p),socket(s),flags(0)
	{
	}
	DataMMSQL::DataMMSQL(const std::string& h,const std::string& u,const std::string& pwd,const std::string& d,unsigned int p,const std::string& s,unsigned long f) : host(h),user(u),password(pwd),DataSource(d),port(p),socket(s),flags(f)
	{
	}
	DataMMSQL::~DataMMSQL()
	{
	}
	
	

	const std::string& DataMMSQL::get_host()const
	{
		return host;
	}
	const std::string& DataMMSQL::get_user()const
	{
		return user;
	}
	const std::string& DataMMSQL::get_password()const
	{
		return password;
	}
	const std::string& DataMMSQL::get_socket()const
	{
		return socket;
	}
	unsigned int DataMMSQL::get_port()const
	{
		return port;
	}
	unsigned long DataMMSQL::get_flags()const
	{
		return flags;
	}
	


	template<> Result<DataMMSQL>::~Result()
	{
		if (result)
		{
			mysql_free_result((MYSQL_RES*)result);
			result = NULL;
		}
	}
		
	template<> size_t Result<DataMMSQL>::number_rows()const
	{
		if (result) return mysql_num_rows((MYSQL_RES*)result);

		return 0;
	}
	template<> void Result<DataMMSQL>::close()
	{
		if (result)
		{
			mysql_free_result((MYSQL_RES*)result);
			result = NULL;
		}
	}
	
	



	
		
	
	
	template<> bool Connection<DataMMSQL>::connect(const DataMMSQL& data, bool a)
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
			//std::cout << "Error : " << mysql_errno(reinterpret_cast<MYSQL*>(connection)) << " : " << mysql_error(reinterpret_cast<MYSQL*>(connection)) << "\n";
			//mysql_close(con);
			connected = false;
			//connection = NULL;
			throw ExceptionSQL(connection, __FILE__, __LINE__);
		}

		if(mysql_autocommit(con,a) == 0) autocommit = a;

		return connected;
	}
	template<> Connection<DataMMSQL>::Connection() : connection((Handle)mysql_init(NULL)), connected(false), autocommit(false)
	{
	}
	template<> Connection<DataMMSQL>::Connection(const DataMMSQL& d, bool a): connection((Handle)mysql_init(NULL)),connected(false), autocommit(a)
	{
		connect(d,a);
	}
	template<> Connection<DataMMSQL>::~Connection()
	{
		if(connection)
		{
			mysql_close(reinterpret_cast<MYSQL*>(connection));
			connection = NULL;
		}		
	}

	template<> Result<DataMMSQL> Connection<DataMMSQL>::execute(const std::string& str)
	{
		if (not connected) throw ExceptionSQL(connection, __FILE__, __LINE__);
		if(not connection) throw ExceptionSQL(connection, __FILE__, __LINE__);

		int ret_query = mysql_query(reinterpret_cast<MYSQL*>(connection), str.c_str());
		
		if (ret_query == -1 and mysql_errno(reinterpret_cast<MYSQL*>(connection)) == 2000) 
		{
			//throw ExceptionQuery("Se genero error desconocido como respuesta, este error podria deverse a un bug",__FILE__,__LINE__);
			throw ExceptionSQL(connection, __FILE__, __LINE__);
		}
		else if (ret_query == 0) 
		{
			return Result<DataMMSQL>(mysql_store_result(reinterpret_cast<MYSQL*>(connection)));
		}
		else
		{
			//std::cout << "Error Maria : " << mysql_errno(reinterpret_cast<MYSQL*>(connection)) << "  " << mysql_error(reinterpret_cast<MYSQL*>(connection)) << "\n";
			//std::cout << "ret_query : " << ret_query << "\n";
			//std::cout << "SQL String : " << str << "\n";
			//return Result<DataMaria>();
			//throw ExceptionQuery("Fallo la ejecucion de la consulta",__FILE__,__LINE__);
			throw ExceptionSQL(connection, __FILE__, __LINE__);
		}		
	}


	
	template<> bool Connection<DataMMSQL>::commit()
	{
		if(connection) if(mysql_commit(reinterpret_cast<MYSQL*>(connection)) == 0) return true;

		return false;
	}
	template<> bool Connection<DataMMSQL>::rollback()
	{
		if(connection) if(mysql_rollback(reinterpret_cast<MYSQL*>(connection)) == 0) return true;

		return false;
	}


	template<> void Connection<DataMMSQL>::close()
	{
		if(connection)
		{
			mysql_close(reinterpret_cast<MYSQL*>(connection));
			connection = NULL;
		}
	}
	template<> bool Connection<DataMMSQL>::ping()
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

namespace oct::cave::v0::mmsql
{
	Result::Result(v0::Result<DataMMSQL>&& r) noexcept
	{	
	}
	/*Result::Result(Result&& r) noexcept : v0::Result<DataMMSQL>(r)
	{

	}*/
	/*Result::Result(Handle&& h) noexcept : v0::Result<DataMMSQL>(h)
	{

	}*/
	Result::~Result()
	{
	}

	void Result::operator =(v0::Result<DataMMSQL>&& r) noexcept
	{
		move(&r, this);
	}
	
}



