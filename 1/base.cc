
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


    Script::Script(const std::filesystem::path& p)
    {
        load(p);
    }
    Script::operator const std::vector<std::string>& () const
    {
        return sql;
    }
    void Script::print(std::ostream& out)
    {
        for(const std::string& str : sql)
        {
            out << str << "\n";
        }
    }
    void Script::create_database(const char* database)
    {
        std::string str;
        str = "CREATE DATABASE ";
        str += "`";
        str += database;
        str += "`;";
        sql.push_back(str);
    }
    void Script::drop_database(const char* database)
    {
        std::string str;
        str = "DROP DATABASE ";
        str += "`";
        str += database;
        str += "`;";
        sql.push_back(str);
    }
    void Script::use(const char* database)
    {
        std::string str;
        str = "USE ";
        str += "`";
        str += database;
        str += "`;";
        sql.push_back(str);
    }
    void Script::create_user(const char *user,bool check_exist,const char* host,const char* password)
    {
        std::string str;
        str = "CREATE USER ";
        if(check_exist) str += "IF NOT EXISTS";
        str += " '";
        str += user;
        str += "'@'";
        str += host;
        str += "' IDENTIFIED BY '";
        str += password;
        str += "';";
        sql.push_back(str);
    }
    void Script::grand_all_privileges(const char *database,const char *user,const char* host,const char* password)
    {
        std::string str;
        str = "GRANT ALL PRIVILEGES ON `";
        str += database;
        str += "`.* TO '";
        str += user;
        str += "'@'%' IDENTIFIED BY '";
        str += password;
        str += "';";
        sql.push_back(str);
    }
    Script& Script::operator << (const char* str)
    {
        sql.push_back(str);

        return *this;
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
            create_database(database);
            break;
        case drop:
            if(not database) break;
            drop_database(database);
            break;
        case use:
            if(not database) break;
            Script::use(database);
            break;
        case basic_header:
            if(not database) break;
            if(not user) break;
            if(not password) break;
            create_user(user,true,"%",password);
            Script::create_database(database);
            grand_all_privileges(database,user,"%",password);
            Script::use(database);
            break;
        }
    }
    void Script::load(const std::filesystem::path& p)
    {
        std::ifstream actual(p);
        if(not actual.good()) throw core::exception("Fallo al abrir el archivo " + p.string());
        std::string strline;

        while(std::getline(actual,strline))
        {
            if(strline.empty() or strline.starts_with("--"))
            {
                strline.clear();
                continue;
            }
            //std::cout << "\tsql : '" << strline << "'\n";
            sql.push_back(strline);
            strline.clear();
        }
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
}
