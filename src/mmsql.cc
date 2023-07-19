

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
#include "oct-core.hh"


#if defined LINUX_ARCH
	#include <mysql/mysql.h>
#elif defined LINUX_GENTOO
	#include <mariadb/mysql.h>
#elif defined LINUX_DEBIAN
	#include <mariadb/mysql.h>
#elif MSYS2
    #include <mariadb/mysql.h>
#elif (defined(_WIN32) || defined(_WIN64))
    #include <mysql/mysql.h>
#else
	#error "Plataforma desconocida."
#endif

namespace cave_current = oct::cave::v0;

namespace oct::cave::v0::mmsql
{
	Data::Data(const std::string& h, const std::string& u, const std::string& pwd) : host(h), user(u), password(pwd), port(3306), flags(0)
	{
	}
	Data::Data(const std::string& h, const std::string& u, const std::string& pwd, const std::string& d) : DataSource(d), host(h), user(u), password(pwd), port(3306), flags(0)
	{
	}
	Data::Data(const std::string& h, const std::string& u, const std::string& pwd, unsigned int p) : host(h), user(u), password(pwd), port(p), flags(0)
	{
	}
	Data::Data(const std::string& h, const std::string& u, const std::string& pwd, const std::string& d, unsigned int p) : DataSource(d), host(h), user(u), password(pwd), port(p), flags(0)
	{
	}
	Data::Data(const std::string& h, const std::string& u, const std::string& pwd, const std::string& d, unsigned int p, const std::string& s) : DataSource(d), host(h), user(u), password(pwd), socket(s), port(p), flags(0)
	{
	}
	Data::Data(const std::string& h, const std::string& u, const std::string& pwd, const std::string& d, unsigned int p, const std::string& s, unsigned long f) : DataSource(d), host(h), user(u), password(pwd), socket(s), port(p), flags(f)
	{
	}
	Data::~Data()
	{
	}



	const std::string& Data::get_host()const
	{
		return host;
	}
	const std::string& Data::get_user()const
	{
		return user;
	}
	const std::string& Data::get_password()const
	{
		return password;
	}
	const std::string& Data::get_socket()const
	{
		return socket;
	}
	unsigned int Data::get_port()const
	{
		return port;
	}
	unsigned long Data::get_flags()const
	{
		return flags;
	}

}

namespace oct::cave::v0
{
    static_assert(sizeof(Handle) == sizeof(MYSQL*),"El manejador no es adecaudo para este driver.");
    static_assert(sizeof(Handle) == sizeof(MYSQL_FIELD*),"El manejador no es adecaudo para este driver.");

	ExceptionDriver::ExceptionDriver(Handle h, const std::string& m, const std::source_location& l) noexcept : oct::core::v3::exception(mysql_errno(reinterpret_cast<MYSQL*>(h)),m, l), handle(h)
	{
	}
	ExceptionDriver::ExceptionDriver(Handle h, const std::source_location& l) noexcept : oct::core::v3::exception(mysql_errno(reinterpret_cast<MYSQL*>(h)),l), handle(h)
	{
	}
	const char* ExceptionDriver::driver_message(Handle) const
	{
		return mysql_error(reinterpret_cast<MYSQL*>(handle));
	}




	template<> Result<char, cave_current::mmsql::Data>::~Result()
	{
		if (result)
		{
			mysql_free_result((MYSQL_RES*)result);
			result = NULL;
		}
	}

	template<> size_t Result<char, cave_current::mmsql::Data>::size()const
	{
		if (result) return mysql_num_rows((MYSQL_RES*)result);

		return 0;
	}
	template<> void Result<char, cave_current::mmsql::Data>::close()
	{
		if (result)
		{
			mysql_free_result((MYSQL_RES*)result);
			result = NULL;
		}
	}
	template<> Row<char,cave_current::mmsql::Data> Result<char, cave_current::mmsql::Data>::next()
	{
		char** str = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
		size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
		Row<char, cave_current::mmsql::Data> row((const char**)str,size);
		return row;
	}
	template<> void Result<char, cave_current::mmsql::Data>::load_fields_info()
	{
		MYSQL_FIELD* field;
		size_t i = 0;
		FieldInfo info;
		fields.reserve(size());
		while((field = mysql_fetch_field(reinterpret_cast<MYSQL_RES*>(result))))
		{
			switch(field->type)
			{
			case MYSQL_TYPE_TINY:
				info.type = Result<char, cave_current::mmsql::Data>::Types::SCHAR;
				break;
			case MYSQL_TYPE_SHORT:
				info.type = Result<char, cave_current::mmsql::Data>::Types::SHORT;
				break;
			case MYSQL_TYPE_LONG:
				info.type = Result<char, cave_current::mmsql::Data>::Types::LONG;
				break;
			case MYSQL_TYPE_LONGLONG:
				info.type = Result<char, cave_current::mmsql::Data>::Types::LONGLONG;
				break;
			case MYSQL_TYPE_FLOAT:
				info.type = Result<char, cave_current::mmsql::Data>::Types::FLOAT;
				break;
			case MYSQL_TYPE_DECIMAL:
			case MYSQL_TYPE_DOUBLE:
				info.type = Result<char, cave_current::mmsql::Data>::Types::DOUBLE;
				break;
			case MYSQL_TYPE_TIMESTAMP:
			case MYSQL_TYPE_DATE:
			case MYSQL_TYPE_TIME:
			case MYSQL_TYPE_DATETIME:
			case MYSQL_TYPE_TIMESTAMP2:
				info.type = Result<char, cave_current::mmsql::Data>::Types::STRING;
				break;
			case MYSQL_TYPE_VARCHAR:
			case MYSQL_TYPE_STRING:
				info.type = Result<char, cave_current::mmsql::Data>::Types::STRING;
				break;
			case MYSQL_TYPE_ENUM:
			case MYSQL_TYPE_SET:
				info.type = Result<char, cave_current::mmsql::Data>::Types::SELECTION;
				break;
			default:
				info.type = Result<char, cave_current::mmsql::Data>::Types::NONE;
			}
			fields.push_back(info);
			i++;
		}
	}






	//Connection<char, mmsql::Data, cave_current::Result<char,mmsql::Data>>
	template<> bool Connection<char, mmsql::Data>::connect(const cave_current::mmsql::Data& data, bool a)
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
			throw ExceptionDriver((Handle)connection,"Fallo la conexion del driver");
		}

		if(mysql_autocommit(con,a) == 0) autocommit = a;

		return connected;
	}
	template<> Connection<char, mmsql::Data>::Connection() : connected(false), connection((Handle)mysql_init(NULL)), autocommit(false)
	{
	}
	template<> Connection<char, mmsql::Data>::Connection(Connection&& c) noexcept
	{
		connected	= c.connected;
		connection	= c.connection;
		autocommit	= c.autocommit;
		c.connection = NULL;
	}
	template<> Connection<char, mmsql::Data>::Connection(const mmsql::Data& d, bool a): connected(false), connection((Handle)mysql_init(NULL)),autocommit(a)
	{
		connect(d,a);
	}
	template<> Connection<char, mmsql::Data>::~Connection()
	{
		if(connection)
		{
			mysql_close(reinterpret_cast<MYSQL*>(connection));
			connection = NULL;
		}
	}

	template<> Result<char,mmsql::Data> Connection<char, mmsql::Data>::execute(const std::string& str)
	{
		if (not connected) throw ExceptionDriver("No se harealizado la cionexion.");
		if (not connection) throw ExceptionDriver("No se ha establesido la cionexion");

		//std::cout << "Connection::execute Step 1\n";

		int ret_query = mysql_query(reinterpret_cast<MYSQL*>(connection), str.c_str());

		//std::cout << "Connection::execute Step 2\n";

		if(ret_query == 0)
		{
			//std::cout << "Connection::execute Step 2.2\n";
			return Result<char,mmsql::Data>(mysql_store_result(reinterpret_cast<MYSQL*>(connection)));
		}

		//std::cout << "Connection::execute Step 3\n";
		throw ExceptionDriver(connection,"La consulta fallo");
	}



	template<> bool Connection<char, mmsql::Data>::commit()
	{
		if(connection) if(mysql_commit(reinterpret_cast<MYSQL*>(connection)) == 0) return true;

		return false;
	}
	template<> bool Connection<char, mmsql::Data>::rollback()
	{
		if(connection) if(mysql_rollback(reinterpret_cast<MYSQL*>(connection)) == 0) return true;

		return false;
	}


	template<> void Connection<char, mmsql::Data>::close()
	{
		if(connection)
		{
			mysql_close(reinterpret_cast<MYSQL*>(connection));
			connection = NULL;
		}
	}
	template<> bool Connection<char, mmsql::Data>::ping()
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
}



