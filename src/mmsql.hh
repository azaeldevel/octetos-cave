#ifndef OCTETOS_COVE_MMSQL_V0_HH
#define OCTETOS_COVE_MMSQL_V0_HH

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

#include "base.hh"



namespace oct::cave::v0
{

class DataMMSQL : public DataSource
{
public:
	DataMMSQL(const std::string& host,const std::string& user,const std::string& password);
	DataMMSQL(const std::string& host,const std::string& user,const std::string& password,const std::string& database);
	DataMMSQL(const std::string& host,const std::string& user,const std::string& password,unsigned int port);
	DataMMSQL(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port);
	DataMMSQL(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port,const std::string& socket);
	DataMMSQL(const std::string& host,const std::string& user,const std::string& password,const std::string& database,unsigned int port,const std::string& socket,unsigned long flags);
	~DataMMSQL();
		
	const std::string& get_host()const;
	const std::string& get_user()const;
	const std::string& get_password()const;
	const std::string& get_socket()const;
	unsigned int get_port()const;
	unsigned long get_flags()const;
	
private:
	std::string host,user,password,socket;
	unsigned int port;
	unsigned long flags;	
};



}


namespace oct::cave::v0::mmsql
{
	typedef cave::v0::DataMMSQL Data;
	typedef cave::v0::Connection<Data> Connection;
	
	class Row : public v0::Row<const char**>
	{
	public:
		Row() = default;
		Row(const char**, size_t z);
		Row(const Row& row);
		Row(const Row&& row);

		const Row& operator =(const Row& obj);

		/*template<typename T> T store(size_t field)
		{
			if (std::is_same<char, T>::value)
			{
				std::cout << "Result::store(index) detected for char\n";
			}
			else if (std::is_same<bool, T>::value)
			{

			}
			else if (std::is_same<int8_t, T>::value)
			{

			}
			else if (std::is_same<const char**, T>::value)
			{

			}
			else if (std::is_same<std::string, T>::value)
			{
				std::cout << "Result::store(index) detected for std::string\n";
				return "";
			}
		}*/
		/*template<typename T> void store(T& v, size_t field)
		{
			if (std::is_same<char, T>::value)
			{
				//std::cout << "Result::store(data,index) detected for char\n";
			}
			else if (std::is_same<bool, T>::value)
			{

			}
			else if (std::is_same<int8_t, T>::value)
			{

			}
			else if (std::is_same<const char**, T>::value)
			{

			}
			else if (std::is_same<std::string, T>::value)
			{
				//std::cout << "Result::store(data,index) detected for std::string\n";
			}
		}*/

		template<typename T> T store(size_t field)
		{
			//throw ExceptionSQL("No se encotro conversion para el objeto.", __FILE__, __LINE__);
		}
		template<typename T> void store(T& v, size_t field)
		{
			//throw ExceptionSQL("No se encotro conversion para el objeto.",__FILE__,__LINE__);
		}
		void store(char& v, size_t field);
		//void store(signed char& v, size_t field);
		//void store(unsigned char& v, size_t field);
		void store(const char*& v, size_t field);
		void store(std::string& v, size_t field);
		//void store(std::wstring& v, size_t field);
		void store(int& v, size_t field);
		void store(unsigned int& v, size_t field);
		void store(short& v, size_t field);
		void store(unsigned short& v, size_t field);
		void store(long& v, size_t field);
		void store(unsigned long& v, size_t field);
		void store(long long& v, size_t field);
		void store(unsigned long long& v, size_t field);
		void store(float& v, size_t field);
		void store(double& v, size_t field);
		void store(long double& v, size_t field);
		void store(bool& v, size_t field);

	private:
		
	};
	/*template<> void Row::store<std::string>(std::string& v, size_t field)
	{
	}*/
	//typedef cave::v0::Result<Data> Result;
	class Result : public v0::Result<DataMMSQL>
	{
	public:
		Result() = default;
		Result(v0::Result<DataMMSQL>&& r) noexcept;
		//Result(Result&& r) noexcept;
		//Result(Handle&& h) noexcept;
		virtual ~Result();

		void operator =(v0::Result<DataMMSQL>&& r) noexcept;

		template<RowContainer R> void store(std::vector<v0::Row<R>>& v)
		{
			v.reserve(number_rows());
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back(v0::Row<R>((const char**)row, size));
				}
				else
				{
					;//error
				}
			}
		}
		template<RowContainer R> void store(std::list<v0::Row<R>>& v)
		{
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back(v0::Row<R>((const char**)row, size));
				}
				else
				{
					;//error
				}
			}
		}


		void store(std::vector<Row>& v)
		{
			v.reserve(number_rows());
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back(Row((const char**)row,size));
				}
				else
				{
					;//error
				}
			}
		}
		template<RowContainer R> void store(std::list<Row>& v)
		{
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back(Row((const char**)row, size));
				}
				else
				{
					;//error
				}
			}
		}


		template<ResultContainer R> void store(std::vector<R>& v)
		{
			v.reserve(number_rows());
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				//size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back((const char**)row);
				}
				else
				{
					;//error
				}
			}
		}
		template<ResultContainer R> void store(std::list<R>& v)
		{
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				//size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back((const char**)row);
				}
				else
				{
					;//error
				}
			}
		}
		template<RowContainer R> void store(std::vector<R>& v)
		{
			v.reserve(number_rows());
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row(reinterpret_cast<MYSQL_RES*>(result));
				//size_t size = mysql_num_fields(reinterpret_cast<MYSQL_RES*>(result));
				if (row)
				{
					v.push_back((const char**)row);
				}
				else
				{
					;//error
				}
			}
		}


		Row next();
	};
}

#endif