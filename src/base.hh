
#ifndef OCTETOS_CAVE_CLIENT_HH
#define OCTETOS_CAVE_CLIENT_HH

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

#ifdef OCTEOTOS_CAVE_ENABLE_DEV
	#include <iostream>
#endif

#include <string>
#include <vector>
#include <list>
#if  (defined(_WIN32) || defined(_WIN64)) && COMPILER_VS
	#include <core/src/Exception-v3.hh>
#elif defined __linux__
	#include <octetos/core/Exception-v3.hh>
	#include <glibmm/i18n.h>
	#include "config.h"
#else
	#error "Plataforma desconocida."
#endif

#if defined LINUX_ARCH
	#include <mysql/mysql.h>
#elif defined LINUX_GENTOO
	#include <mariadb/mysql.h>
#elif defined LINUX_DEBIAN
	#include <mariadb/mysql.h>
#elif (defined(_WIN32) || defined(_WIN64)) && COMPILER_VS
    #include <mysql/mysql.h>
#elif MSYS2
    #include <mariadb/mysql.h>
#else
	#error "Plataforma desconocida."
#endif

namespace oct::cave::v0
{

	typedef void* Handle;
	typedef unsigned long index;

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

class ExceptionSQL : public core::v3::Exception
{
public:

public:
	ExceptionSQL(Handle);
	ExceptionSQL(const ExceptionQuery&);

	virtual ~ExceptionSQL();

	virtual const char* what() const noexcept;
private:
	Handle handle;
};


enum class Source
{
	none,
	mmsql,
	maria,
	mysql
};
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
	Result(Result<D>&& r) noexcept
	{
		result = r.result;
		r.result = NULL;
		//std::cout << "Result(Result<D>&& " << result << ")\n";
	}
	Result(const Result<D>&) 
	{
		throw ExceptionResult("No puede ser copiado este objeto",__FILE__,__LINE__);
	}
	Result(Handle&& h) noexcept
	{
		result = h;
		//std::cout << "Result(Handle&& " << result << ")\n";
	}
	~Result();

	void operator =(Result&& r) noexcept
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
		std::string srtsql;
		srtsql.reserve(20 + fields.size() + table.size());
		srtsql = "SELECT ";
		srtsql += fields;
		srtsql += " FROM ";
		srtsql += table;
		srtsql += ";";
		//std::cout << srtsql << "\n";

		return execute(srtsql);
	}
	Result<Data> select(const std::string& fields,const std::string& table,const std::string& where)
	{
		std::string srtsql;
		srtsql.reserve(30 + fields.size() + table.size() + where.size());
		srtsql = "SELECT ";
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
		std::string srtsql;
		size_t reserved = 30 + table.size() + where.size();
		for(const std::string& s : list)
		{
			reserved += s.size() + 1;//el tamaño del estring mas una coma
		}
		srtsql.reserve(reserved);
		
		srtsql = "SELECT ";
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