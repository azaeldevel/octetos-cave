
#ifndef OCTETOS_CAVE_V1_BASE_HH
#define OCTETOS_CAVE_V1_BASE_HH

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
#include <core/3/Exception.hh>
#include <core/3/array.hh>
#include <core/3/numbers.hh>



namespace oct::cave::v1
{
    namespace core = oct::core::v3;

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


	class ExceptionDriver : public core::exception
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
	template<class S> concept starable = std::is_constructible_v<S, const char**> && std::is_default_constructible<S>::value && std::is_move_constructible_v<S> && !row_string<S>;
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
			v = core::to_number<signed char>(row[field]);
		}
		void store(unsigned char& v, size_t field)
		{
			v = core::to_number<unsigned char>(row[field]);
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
			v = core::to_number<int>(row[field]);
		}
		void store(unsigned int& v, size_t field)
		{
			v = core::to_number<unsigned int>(row[field]);
		}
		void store(short& v, size_t field)
		{
			v = core::to_number<short>(row[field]);
		}
		void store(unsigned short& v, size_t field)
		{
			v = core::to_number<unsigned short>(row[field]);
		}
		void store(long& v, size_t field)
		{
			v = core::to_number<long>(row[field]);
		}
		void tore(unsigned long& v, size_t field)
		{
			v = core::to_number<unsigned long>(row[field]);
		}
		void store(long long& v, size_t field)
		{
			v = core::to_number<long long>(row[field]);
		}
		void store(unsigned long long& v, size_t field)
		{
			v = core::to_number<unsigned long long>(row[field]);
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
			v = (bool)core::to_number<unsigned char>(row[field]);
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
		void store(core::array<Row<CB, DS>>& v)
		{
			for (size_t i = 0; i < size(); i++)
			{
				v.push_back(next());
			}
		}

		template<starable S> void store(std::vector<S>& v)
		{
			v.reserve(size());
			Row<CB, DS> row;
			for (size_t i = 0; i < size(); i++)
			{
				row = next();
				v.push_back((const CB**)row);
			}
		}
		template<starable S> void store(std::list<S>& v)
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



	template <typename T> concept is_Result_instation =
		std::is_same_v<
		std::remove_const_t<T>,
		Result<typename T::char_type, typename T::data_type>>;
	template <typename T> concept is_Result_derived = std::derived_from<std::remove_const_t<T>, Result<typename T::char_type, typename T::data_type>>;
	template <typename T> concept result = is_Result_instation<T> || is_Result_derived<T>;



	template <typename T> concept Selectable = requires(T t)
	{
	    T::select_fields();
		T::table();
	} && starable<T>;
	template <typename T> concept Insertable = requires(T t)
	{
	    T::insert_fields();
		T::table();
	    t.insert_values();
	};
	template <typename T> concept Updatable = requires(T t)
	{
	    T::update_fields();
		T::table();
	    t.update_values();
	};
	template <typename T> concept Removable = requires(T t)
	{
	    T::fields();
		T::table();
	    t.values();
	};

	struct Query
	{

	};
	struct Select : public Query
	{

	};
	struct Insert : public Query
	{

	};
	struct Update : public Query
	{

	};

	struct Delete : public Query
	{

	};


	namespace names
	{
        template<class T = std::string> class Table : public T
        {

        public:
            Table()= default;
            Table(const char* str) : T(str)
            {
            }
            Table(const T& str) : T(str)
            {
            }

        };
        template<class T = std::string> class Field : public T
        {

        };
	}

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

		RS execute(const char*);
		RS execute(const std::string& str)
		{
		    return execute(str.c_str());
		}

		//Por string
        RS select(const char* table)
		{
			std::string srtsql = "SELECT * ";
            srtsql += " FROM ";
            srtsql += table;
			//std::cout << srtsql << "\n";

			return execute(srtsql.c_str());
		}
        RS select(const char* table,const char* fields)
		{
			std::string srtsql = "SELECT ";
			srtsql += fields;
            srtsql += " FROM ";
            srtsql += table;
			//std::cout << srtsql << "\n";

			return execute(srtsql.c_str());
		}
        RS select(const char* table,const char* fields,const char* where)
		{
			std::string srtsql = "SELECT ";
			srtsql += fields;
            srtsql += " FROM ";
            srtsql += table;
            srtsql += " WHERE ";
            srtsql += where;
			//std::cout << srtsql << "\n";

			return execute(srtsql.c_str());
		}
        RS select(const char* table,const char* fields,const char* where, size_t limit)
		{
			std::string srtsql = "SELECT ";
			srtsql += fields;
            srtsql += " FROM ";
            srtsql += table;
            srtsql += " WHERE ";
            srtsql += where;
            srtsql += " LIMIT ";
            srtsql += std::to_string(limit);
			//std::cout << srtsql << "\n";

			return execute(srtsql.c_str());
		}

		//Por contenedor
        template<Selectable SC> void select(std::vector<SC>& rs)
		{
			std::string srtsql = "SELECT " + SC::select_fields() + " FROM " + SC::table();
            RS result = execute(srtsql.c_str());
            if(not result) return;
            for(size_t i = 0; i < result.size(); i++)
            {
                rs.push_back(result.next());
            }
		}

		//Por nombres
        RS select(const names::Table<std::string>& table)
		{
			std::string srtsql = "SELECT * ";
            srtsql += " FROM ";
            srtsql += table;
			//std::cout << srtsql << "\n";

			return execute(srtsql.c_str());
		}



        //por string
        RS insert(const char* str)
		{
			return execute(str);
		}
        template<core::natural ID> RS insert(const char* str, ID& id);
        RS insert(const std::string& str)
		{
			return insert(str.c_str());
		}
        RS insert(const char* table,const char* fields,const char* values)
		{
			std::string srtsql = "INSERT INTO ";
            srtsql += table;
			srtsql += "(";
			srtsql += fields;
			srtsql += ") ";
            srtsql += "VALUES(";
            srtsql += values;
            srtsql += ")";
			//std::cout << srtsql << "\n";

			return insert(srtsql);
		}


		RS update(const char* str)
		{
			return execute(str);
		}
		RS update(const std::string& str)
		{
			return update(str.c_str());
		}
        RS update(const char* table,const char* sets)
		{
			std::string srtsql = "UPDATE ";
            srtsql += table;
			srtsql += " SET ";
			srtsql += sets;
			//std::cout << srtsql << "\n";

			return update(srtsql);
		}
        RS update(const char* table,const char* sets,const char* where)
		{
			std::string srtsql = "UPDATE ";
            srtsql += table;
			srtsql += " SET ";
			srtsql += sets;
			srtsql += " WHERE ";
			srtsql += where;
			//std::cout << srtsql << "\n";

			return update(srtsql);
		}



		RS remove(const char* str)
		{
			return execute(str);
		}
		RS remove(const std::string& str)
		{
			return remove(str.c_str());
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
