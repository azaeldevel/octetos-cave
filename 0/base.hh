
#ifndef OCTETOS_CAVE_V0_BASE_HH
#define OCTETOS_CAVE_V0_BASE_HH

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
#include <source_location>
/*#if  (defined(_WIN32) || defined(_WIN64))
	#include <core/src/Exception-v3.hh>
	#include <cave/src/oct-core.hh>
#elif defined __linux__ && IDE_CODEBLOCKS
	#include <core/src/Exception-v3.hh>
	#include <cave/src/oct-core.hh>
#elif defined __linux__
	#include <octetos/core/Exception-v3.hh>
	#include <cave/src/oct-core.hh>
	#include "config.h"
#else
	#error "Plataforma desconocida."
#endif*/

#include <core/3/Exception.hh>
#include "oct-core.hh"

namespace core_here = oct::core::v3;

namespace oct::cave::v0
{

	typedef void* Handle;
	typedef unsigned long index;



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


	class ExceptionDriver : public core_here::exception
	{
	public:

	public:
		ExceptionDriver(const ExceptionDriver&) noexcept = default;

		ExceptionDriver(Handle handle, const std::source_location& location = std::source_location::current()) noexcept;
		ExceptionDriver(Handle handle, const std::string& message, const std::source_location& location = std::source_location::current()) noexcept;
		ExceptionDriver(const std::string& message, const std::source_location& location = std::source_location::current()) noexcept;

		virtual const char* what() const noexcept;
	private:
		Handle handle;
		mutable std::string description;
		const char* driver_message(Handle)const;
	};

	template<class T> concept char_base = std::is_same<char, T>::value || std::is_same<wchar_t, T>::value;
	template<class R> concept row_string = std::is_same<const char*, R>::value || std::is_same<const wchar_t*, R>::value;
	//Contenmedr tipo A, rquiere un contructor para el array de c-string
	template<class S> concept container_A = std::is_constructible_v<S, const char**> && std::is_default_constructible<S>::value && std::is_move_constructible_v<S> && !row_string<S>;
	template<class T> concept datasource = std::derived_from<T, DataSource> || std::is_same<T, DataSource>::value;


	template<char_base CB, datasource DS>
	class Row
	{
	public:
		using char_type = CB;
		using data_type = DS;

	public:
		Row() : row(NULL), size(0)
		{
		}
		Row(const CB** r,size_t z) : row(r), size(z)
		{
		}
		Row(const Row& r) : row(r.row),size(r.size)
		{
		}
		Row(Row&& r) noexcept : row(r.row), size(r.size)
		{
		}
		const Row& operator =(const Row& obj)
		{
			row = obj.row;
			size = obj.size;

			return *this;
		}

		inline const CB* operator[] (size_t i)const
		{
			return i < size ? row[i] : NULL;
		}
		explicit operator const CB** ()const
		{
			return row;
		}

		/**
		*\brief convierte el datos desde la represenacion de BD hacia un valor C++ elegido
		* \param v referencia modificable donde se corocalo el valor
		*/
		template<typename T> void store(T& v, size_t field);
		void store(CB& v, size_t field)
		{
			v = row[field][0];
		}
		void store(signed char& v, size_t field)
		{
			v = oct::core::atoi<signed char>(row[field]);
		}
		void store(unsigned char& v, size_t field)
		{
			v = oct::core::atoi<unsigned char>(row[field]);
		}
		void store(const CB*& v, size_t field)
		{
			v = row[field];
		}
		void store(std::string& v, size_t field)
		{
			v = row[field];
		}
		/*void store(std::wstring& v, size_t field)
		{
			v = r[field];
		}*/
		void store(int& v, size_t field)
		{
			v = oct::core::atoi<int>(row[field]);
		}
		void store(unsigned int& v, size_t field)
		{
			v = oct::core::atoi<unsigned int>(row[field]);
		}
		void store(short& v, size_t field)
		{
			v = oct::core::atoi<short>(row[field]);
		}
		void store(unsigned short& v, size_t field)
		{
			v = oct::core::atoi<unsigned short>(row[field]);
		}
		void store(long& v, size_t field)
		{
			v = oct::core::atoi<long>(row[field]);
		}
		void tore(unsigned long& v, size_t field)
		{
			v = oct::core::atoi<unsigned long>(row[field]);
		}
		void store(long long& v, size_t field)
		{
			v = oct::core::atoi<long long>(row[field]);
		}
		void store(unsigned long long& v, size_t field)
		{
			v = oct::core::atoi<unsigned long long>(row[field]);
		}
		void store(float& v, size_t field)
		{
			v = std::stof(row[field]);
		}
		void store(double& v, size_t field)
		{
			v = std::stod(row[field]);
		}
		void store(long double& v, size_t field)
		{
			v = std::stold(row[field]);
		}
		void store(bool& v, size_t field)
		{
			v = (bool)oct::core::atoi<unsigned char>(row[field]);
		}

		template<typename T> void store(T& v, const char* field);
		template<typename T> void store(T& v, const std::string& field);

		/**
		*\brief conveierte el campo al tipo de dato correspondiente
		*/
		template<typename T> T store(size_t field);


	protected:
		const CB** row;
		size_t size;

		/**
		*\brief usado para operacion de elision
		*/
		static void copy(Row* origin, Row* dest)
		{
			dest->row = origin->row;
			dest->size = origin->size;
		}
	};

	template <typename T> concept is_Row_instation = std::is_same_v<std::remove_const_t<T>, Row<typename T::char_type, typename T::data_type>>;
	template <typename T> concept is_Row_derived = std::derived_from<std::remove_const_t<T>, Row<typename T::char_type, typename T::data_type>>;
	template <typename T> concept row = is_Row_instation<T> || is_Row_derived<T>;


	template<char_base CB, datasource DS> class Result
	{
	public:
		using char_type = CB;
		using data_type = DS;

		enum class Types
		{
			NONE,
			BOOL,
			CHAR,
			SCHAR,
			UCHAR,
			CSTRING,
			STRING,
			INTEGER,
			UINTEGER,
			SHORT,
			USHORT,
			LONG,
			ULONG,
			LONGLONG,
			ULONGLONG,
			FLOAT,
			DOUBLE,
			LONGDOUBLE,
			SELECTION,

		};

		struct FieldInfo
		{
			Types type;
		};

	public:
		Result() : result(NULL)
		{
			//std::cout << "Result()\n";
		}
		Result(Result&& r) noexcept : result(NULL)
		{
			result = r.result;
			r.result = NULL;
			//std::cout << "Result(Result<D>&& " << result << ")\n";
			load_fields_info();
		}
		Result(Handle&& h) noexcept : result(NULL)
		{
			result = h;
			//std::cout << "Result(Handle&& " << result << ")\n";
			load_fields_info();
		}
		Result(const Result&) = delete;
		virtual ~Result();

		void operator =(Result&& r) noexcept
		{
			result = r.result;
			//std::cout << "Result& operator =(Result&&  " << result << ")\n";
			r.result = NULL;
			load_fields_info();
		}
		const Result& operator =(const Result&) = delete;
		bool is_stored() const
		{
			return (result ? true : false);
		}
		operator bool() const
		{
		    return is_stored();
		}


		void close();
		size_t size()const;

		Row<CB,DS> next();

		void store(std::vector<Row<CB, DS>>& v)
		{
			v.reserve(size());
			for (size_t i = 0; i < size(); i++)
			{
				v.push_back(next());
			}
		}
		void store(std::list<Row<CB, DS>>& v)
		{
			for (size_t i = 0; i < size(); i++)
			{
				v.push_back(next());
			}
		}
		template<container_A S> void store(std::vector<S>& v)
		{
			v.reserve(size());
			Row<CB, DS> row;
			for (size_t i = 0; i < size(); i++)
			{
				row = next();
				v.push_back((const CB**)row);
			}
		}
		template<container_A S> void store(std::list<S>& v)
		{
			Row<CB, DS> row;
			for (size_t i = 0; i < size(); i++)
			{
				row = next();
				v.push_back((const CB**)row);
			}
		}
		void store(std::vector<const CB**>& v)
		{
			v.reserve(size());
			Row<CB, DS> row;
			for (size_t i = 0; i < size(); i++)
			{
				row = next();
				v.push_back((const CB**)row);
			}
		}
		void store(std::list<const CB**>& v)
		{
			Row<CB, DS> row;
			for (size_t i = 0; i < size(); i++)
			{
				row = next();
				v.push_back((const CB**)row);
			}
		}
;
		/**
		*\brief Determina el type de datos del campo
		*/
		/*Types get_type(size_t field) const
		{
			if (field < size()) return fields[field].type;

			return Types::NONE;
		}*/

		/*const std::vector<FieldInfo>& fields_info() const
		{
			return fields;
		}*/
	private:
		//std::vector<FieldInfo> fields;
		void load_fields_info();

	protected:
		Handle result;

		static void move(Result* origin, Result* dest)
		{
			dest->result = origin->result;
			origin->result = NULL;
		}
		static void set(Result* dest, Handle r)
		{
			dest->result = r;
		}
	};

	template<char_base CB, datasource DS> const char * to_string(typename Result<CB,DS>::Types const type)
	{
		switch (type)
		{
		case Result<CB, DS>::Types::CHAR: return "char";
		case Result<CB, DS>::Types::UCHAR: return "unsigned char";
		case Result<CB, DS>::Types::SCHAR: return "signed char";
		case Result<CB, DS>::Types::SHORT: return "short";
		case Result<CB, DS>::Types::USHORT: return "unsigned short";
		case Result<CB, DS>::Types::INTEGER: return "int";
		case Result<CB, DS>::Types::UINTEGER: return "unsigned int";
		case Result<CB, DS>::Types::LONG: return "long";
		case Result<CB, DS>::Types::ULONG: return "unsigned long";
		case Result<CB, DS>::Types::LONGLONG: return "long long";
		case Result<CB, DS>::Types::ULONGLONG: return "unsigned long long";
		case Result<CB, DS>::Types::FLOAT: return "float";
		case Result<CB, DS>::Types::DOUBLE: return "double";
		case Result<CB, DS>::Types::LONGDOUBLE: return "long double";
		case Result<CB, DS>::Types::SELECTION: return "std::vector<std::string>";
		}
	}

	template <typename T> concept is_Result_instation =
		std::is_same_v<
		std::remove_const_t<T>,
		Result<typename T::char_type, typename T::data_type>>;
	template <typename T> concept is_Result_derived = std::derived_from<std::remove_const_t<T>, Result<typename T::char_type, typename T::data_type>>;
	template <typename T> concept result = is_Result_instation<T> || is_Result_derived<T>;

	template <typename T> concept ContainerSelectionStorable = requires(T t)
	{
	    T::fields();
		T::table();
		//TODO: verificar que el contenerdor tenga contructor para row
	};
	template <typename T> concept ContainerInsertable = requires(T t)
	{
	    T::fields();
		T::table();
	    t.values();
	};

	template<char_base CB, datasource DS, result RS = Result<CB,DS>> class Connection
	{
	public:
		using char_type = CB;
		using data_type = DS;
		using result_type = RS;

	public:
		Connection();
		Connection(Connection&&) noexcept;
		Connection(const DS& data, bool autocommit);
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

		RS execute(const std::string&);
		bool insert(const std::string&);
		bool update(const std::string&);


		RS select(const std::string& fields,const std::string& table)
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
		/**
		*\brief SQL clause select.
		*
		*/
		RS select(const std::string& fields,const std::string& table,const std::string& where)
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
		RS select(const std::vector<std::string>& list,const std::string& table,const std::string& where)
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
		template<ContainerSelectionStorable CS> bool select(std::vector<CS>& c)
		{
		    std::string srtsql = "SELECT " + CS::fields() + " FROM " + CS::table();
            RS result = execute(srtsql);
            if(not result) return false;
            c.reserve(result.size());
            for(size_t i = 0; i < result.size(); i++)
            {
                c.push_back(result.next());
            }

		    return true;
		}
		template<ContainerSelectionStorable CS> bool select(std::vector<CS>& c,const std::string& where)
		{
		    std::string srtsql = "SELECT " + CS::fields() + " FROM " + CS::table()  + " WHERE " + where;
			RS result = execute(srtsql);
			if (not result) return false;
			c.reserve(result.size());
			for (size_t i = 0; i < result.size(); i++)
			{
				c.push_back(result.next());
			}

			return true;
		}
		template<ContainerSelectionStorable CS> bool select(std::list<CS>& c)
		{
			std::string srtsql = "SELECT " + CS::fields() + " FROM " + CS::table();
			RS result = execute(srtsql);
			if (not result) return false;

			for (size_t i = 0; i < result.size(); i++)
			{
				c.push_back(result.next());
			}

			return true;
		}
		template<ContainerSelectionStorable CS> bool select(std::list<CS>& c, const std::string& where)
		{
			std::string srtsql = "SELECT " + CS::fields() + " FROM " + CS::table() + " WHERE " + where;
			RS result = execute(srtsql);
			if (not result) return false;

			for (size_t i = 0; i < result.size(); i++)
			{
				c.push_back(result.next());
			}

			return true;
		}

		RS insert(const std::string& fields,const std::string& values,const std::string& table)
		{
            std::string srtsql = "INSERT INTO " + table + "(" + fields + ")" + " VALUES(" + values + ")";
            return execute(srtsql);
		}
		template<ContainerInsertable CI> RS insert(const CI& c)
		{
            std::string srtsql = "INSERT INTO " + CI::table() + "(" + CI::fields() + ")" + " VALUES(" + c.values() + ")";
            return execute(srtsql);
		}


		RS update(const std::vector<std::string>& sets, const std::string& table)
		{
			if (sets.empty()) return RS();

			std::string srtsql = "UPDATE " + table + " SET ";
			for (size_t i = 0; i < sets.size() - 1; i++)
			{
				srtsql += sets[i] + ",";
			}
			srtsql += sets.back();

			return execute(srtsql);
		}
		RS update(const std::vector<std::string>& sets, const std::string& table, const std::string& where)
		{
			if (sets.empty()) return RS();

			std::string srtsql = "UPDATE " + table + " SET ";
			for (size_t i = 0; i < sets.size() - 1; i++)
			{
				srtsql += sets[i] + ",";
			}
			srtsql += sets.back();
			srtsql  += " WHERE = " + where;
			return execute(srtsql);
		}
		RS update(const std::string& sets, const std::string& table)
		{
			if (sets.empty()) return RS();
			if (table.empty()) return RS();

			std::string srtsql = "UPDATE " + table + " SET " + sets;

			return execute(srtsql);
		}


		RS remove(const std::string& table, const std::string& where)
		{
			std::string srtsql = "DELETE FROM " + table + " WHERE " + where;
			return execute(srtsql);
		}

		bool begin();
		bool commit();
		bool rollback();

		bool connect(const DS&, bool autocommit);
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
