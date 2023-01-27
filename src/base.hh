
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
#include <type_traits>
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
		ExceptionSQL(Handle,const char* filename, unsigned int line);
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
	
	
	template<class R> concept RowContainer = std::is_same<const char**, R>::value || std::is_same<const wchar_t**, R>::value;
	template<class S> concept ResultContainer = std::is_constructible_v<S, const char**> && std::is_default_constructible<S>::value && std::is_move_constructible_v<S> && !RowContainer<S>;

	template<RowContainer R>
	class Row
	{
	public:
		Row() : r(NULL), size(0)
		{
		}
		Row(R r,size_t z) : size(z)
		{
			this->r = r;
		}
		Row(const Row& row) : r(row.r),size(row.size)
		{
		}
		Row(const Row&& row) : r(row.r), size(row.size)
		{
		}
		const Row& operator =(const Row& obj)
		{
			r = obj.r;
			size = obj.size;
		}

		inline const char* operator[] (size_t i)const
		{
			return r[i];
		}

		template<typename T> void store(T& v, size_t field);
		template<typename T> void store(T& v, const char* field);
		template<typename T> void store(T& v, const std::string& field);
		
	protected:
		R r;
		size_t size;

		static void copy(Row* origin, Row* dest)
		{
			dest->r = origin->r;
			dest->size = origin->size;
		}
	};

	template<typename D> class Result
	{
	public:
		Result() : result(NULL)
		{
			//std::cout << "Result()\n";
		}
		Result(Result&& r) noexcept
		{
			result = r.result;
			r.result = NULL;
			//std::cout << "Result(Result<D>&& " << result << ")\n";
		}
		Result(Handle&& h) noexcept
		{
			result = h;
			//std::cout << "Result(Handle&& " << result << ")\n";
		}
		Result(const Result&) 
		{
			throw ExceptionResult("No puede ser copiado este objeto",__FILE__,__LINE__);
		}
	
		virtual ~Result();

		void operator =(Result&& r) noexcept
		{
			result = r.result;
			//std::cout << "Result& operator =(Result&&  " << result << ")\n";
			r.result = NULL;
		}
		const Result& operator =(const Result&)
		{
			throw ExceptionResult("No puede ser copiado este objeto",__FILE__,__LINE__);
		}
		bool is_stored() const
		{
			return (result ? true : false);
		}

		void close();
		size_t number_rows()const;

		template<RowContainer S> Row<S> next();

		template<RowContainer S> void store(std::vector<Row<S>>& v);
		template<RowContainer S> void store(std::list<Row<S>>& v);
		template<ResultContainer S> void store(std::vector<S>& v);
		template<ResultContainer S> void store(std::list<S>& v);
		template<RowContainer S> void store(std::vector<S>& v);
		template<RowContainer S> void store(std::list<S>& v);

	private:

	protected:
		Handle result;

		void move(Result<D>* origin, Result<D>* dest)
		{
			dest->result = origin->result;
			origin->result = NULL;
		}
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