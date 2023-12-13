#ifndef OCTETOS_CAVE_V1_MMSQL_HH
#define OCTETOS_CAVE_V1_MMSQL_HH

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

namespace oct::cave::v1
{

}

namespace oct::cave::v1::mmsql
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

#ifdef OCTEOTOS_CAVE_ENABLE_TDD_V1
		void print(std::ostream& o)const
		{
            o << host << ":" << port << "/" << user << "/" << database << "\n";
		}
#endif // OCTEOTOS_CAVE_ENABLE_TDD_V1

	private:
		std::string host, user, password, socket;
		unsigned int port;
		unsigned long flags;
	};

	typedef oct::cave::v1::Connection<char,Data> Connection;
	typedef oct::cave::v1::Result<char,Data> Result;


    bool create_database(Connection& conn,const std::string& database);
    bool execute(Connection& conn,const std::vector<std::filesystem::path>& sources);
    void copy_chars(const std::string& from,std::string& to);

}

#endif
