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


template<class S>
concept RowMMSQL = requires(S s)
{
	s = (const char**)0;//operator de asignacio para arreglo de c strings
};


}


namespace oct::cave::v0::mmsql
{
	typedef cave::v0::DataMMSQL Data;
	typedef cave::v0::Connection<Data> Connection;
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

		template<RowMMSQL R> void store(std::vector<R>& v)
		{
#ifdef OCTEOTOS_CAVE_ENABLE_DEV
			//std::cout << "template<typename S> void store(std::vector<S>& v)\n";
#endif
			v.resize(number_rows());
			char** row;
			for (index i = 0; i < number_rows(); i++)
			{
				row = mysql_fetch_row((MYSQL_RES*)result);
				if (row)
				{
					v.at(i) = (const char**)row;
				}
				else
				{
					;//error
				}
			}
		}
	};
}

#endif