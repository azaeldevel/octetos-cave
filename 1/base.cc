
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


ExceptionDriver::ExceptionDriver(const std::string& m, const std::source_location& l) noexcept : core::exception(m,l), handle(NULL)
{
}

const char* ExceptionDriver::what() const noexcept
{
	if (description.empty())
	{
		description = _location.file_name();
		description += " : ";
		description += std::to_string(_location.line()) + " ";
		if (not _message.empty()) description += _message + " ";
		if (_code_flag)
        {
            description += "\n\tCode(" + std::to_string(_code);
            if (handle)
            {
                description +=  "): ";
                description +=  driver_message(handle);
            }
        }
	}

	return description.c_str();
}

DataSource::DataSource(const std::string& d) : database(d)
{
}
const std::string& DataSource::get_database()const
{
	return database;
}


    Database::Database(Database::Type t,const char* d) : type(t),database(d)
    {
        build();
    }
    Database::Database(Database::Type t,const char* d,const char* u,const char* p) : type(t),database(d),user(u),password(p)
    {
        build();
    }
    void Database::build()
    {
        std::string str;
        switch(type)
        {
        case create:
            if(not database) break;
            str = "CREATE DATABASE ";
            str += "`";
            str += database;
            str += "`;";
            break;
        case drop:
            if(not database) break;
            str = "DROP DATABASE ";
            str += "`";
            str += database;
            str += "`;";
            break;
        case use:
            if(not database) break;
            str = "USE ";
            str += "`";
            str += database;
            str += "`;";
            break;
        case basic_header:
            if(not database) break;
            if(not user) break;
            if(not password) break;
            str = "CREATE  USER IF NOT EXISTS '";
            str += user;
            str += "'@'%' IDENTIFIED BY '";
            str += password;
            str += "';";
            sql.push_back(str);
            str = "CREATE DATABASE `";
            str += database;
            str += "`;";
            sql.push_back(str);
            str = "GRANT ALL PRIVILEGES ON `";
            str += database;
            str += "`.* TO '";
            str += user;
            str += "'@'%' IDENTIFIED BY '";
            str += password;
            str += "';";
            sql.push_back(str);
            str = "FLUSH PRIVILEGES;";
            sql.push_back(str);
            str = "USE `";
            str += database;
            str += "`;";
            sql.push_back(str);
            break;
        }
    }
    Database::operator const std::vector<std::string>& () const
    {
        return sql;
    }

    std::vector<std::string> split(const std::string& source,bool log)
    {
        std::vector<std::string> ls(1);
        size_t last_index;
        for(size_t i = 0; i < source.size(); i++)
        {
            if(source[i] == '\n') ls.push_back("");
            last_index = (ls.back().size() == 0 ? 0 : ls.back().size() - 1);
            ls.back().insert(last_index,source[i],1);
        }

        return ls;
    }
    void print(std::ostream& out,const std::vector<std::string>& list)
    {
        for(const std::string& str : list)
        {
            out << str << "\n";
        }
    }
}
