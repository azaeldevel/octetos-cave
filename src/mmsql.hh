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
}

namespace oct::cave::v0::mmsql
{
	class Data : public DataSource
	{
	public:
		Data(const std::string& host, const std::string& user, const std::string& password);
		Data(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
		Data(const std::string& host, const std::string& user, const std::string& password, unsigned int port);
		Data(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port);
		Data(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port, const std::string& socket);
		Data(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port, const std::string& socket, unsigned long flags);
		~Data();

		const std::string& get_host()const;
		const std::string& get_user()const;
		const std::string& get_password()const;
		const std::string& get_socket()const;
		unsigned int get_port()const;
		unsigned long get_flags()const;

	private:
		std::string host, user, password, socket;
		unsigned int port;
		unsigned long flags;
	};

	class Result : public cave_current::Result<char, Data>
	{
	public:
		Result() = default;
		Result(Result&& r) noexcept;
		Result(Handle&& h) noexcept;
		Result(const Result&) = delete;
		virtual ~Result();

		void operator =(cave_current::Result<char, cave_current::mmsql::Data>&& r) noexcept;
		void operator =(Result&& r) noexcept;
		const Result& operator =(const Result& r) = delete;

		//string to fetched row
		void store(std::vector<Row<char, Data>>& v);
		//string to fetched row
		void store(std::list<Row<char, Data>>& v);

		//struct to load data in c++ format
		template<container_A R> void store(std::vector<R>& v)
		{
			v.reserve(size());
			char** row;
			for (index i = 0; i < size(); i++)
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
		//struct to load data in c++ format
		template<container_A R> void store(std::list<R>& v)
		{
			char** row;
			for (index i = 0; i < size(); i++)
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

		void store(std::vector<const char**>& v);
		void store(std::list<const char**>& v);

		Row<char,Data> next();
	};


	typedef cave_current::Connection<char,Data,Result> Connection;
	/*class Connection : public cave_current::Connection<const char*, Data>
	{
	public:
		Connection(const Data& data, bool autocommit);

		bool connect(const Data&, bool autocommit);
	};*/
}

#endif
