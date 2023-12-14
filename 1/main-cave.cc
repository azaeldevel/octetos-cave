
/*
 * main.cc
 * Copyright (C) 2022 Azael Reyes <azael.devel@gmail.com>
 *
 * muposys-core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * muposys-core is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <stdlib.h>
#include <list>
#include <algorithm>

#if defined (__linux__) && defined (IDE_CODEBLOCKS)
	#include "mmsql.hh"
#elif defined(_WIN32) || defined(_WIN64)

#elif defined (__linux__)
	#include "mmsql.hh"
#else
	#error "Plataforma desconocida"
#endif

namespace cave = oct::cave::v1;


int create(int argc, char* argv[]);
int create_database(int argc, char* argv[]);
int repository(int argc, char* argv[]);
int create_user(int argc, char* argv[]);

const std::string cmdpwd("[require]");

int main(int argc, char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
        //std::cout << "Main : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
        if(strcmp("import",argv[i]) == 0)
        {
            if(i < argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            //return import(argc - i,&argv[i]);
            return EXIT_FAILURE;
        }
        else if(strcmp("create",argv[i]) == 0)
        {
            return create(argc - i,&argv[i]);
        }
        else if(strcmp("repository",argv[i]) == 0)
        {
            return repository(argc - i,&argv[i]);
        }
    }

	return EXIT_SUCCESS;
}

int repository(int argc, char* argv[])
{
    std::filesystem::path dir,package = "ocpk";
    std::string user = "root",password,database, host = "localhost";
    int port = 3306;

    for(int i = 0; i < argc; i++)
    {
        //std::cout << "Database : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
        if(strcmp("--password",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para pakage" << std::endl;
                return EXIT_FAILURE;
            }

            password = argv[++i];
        }
        else if(strcmp("--database",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys database" << std::endl;
                return EXIT_FAILURE;
            }
            //std::cout << "\tDatabase : \t\t" << argv[i+1] << std::endl;

            database = argv[++i];
        }
        else if(strcmp("--host",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            host = argv[++i];
        }
        else if(strcmp("--user",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            user = argv[++i];
        }
        else if(strcmp("--port",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }
            port = std::stoi(argv[++i]);
        }
        else if(strcmp("repository",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            dir = argv[++i];
        }
        else if(strcmp("--package",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            package = argv[++i];
        }
    }

    if(dir.empty())
    {
        std::cerr << "No se espesifico un reposistorio para trabajar" << std::endl;
        return EXIT_FAILURE;
    }
    if(not std::filesystem::exists(dir))
    {
        std::cerr << "No se encuentre el archivo " << dir << std::endl;
        return EXIT_FAILURE;
    }

    std::fstream mpk(dir/package, std::ios::in);
    std::string strline,strcmd,strparams;
    std::stringstream sline;
    std::vector<std::filesystem::path> sources;

    while(std::getline(mpk,strline))
    {
        sline.clear();
        sline << strline;

        std::getline(sline,strcmd,':');
        std::getline(sline,strparams,':');
        //std::cout << strcmd << ":" << strparams << std::endl;

        if(strcmd.compare("source") == 0)
		{
		    sources.push_back(dir/strparams);
		    if(not std::filesystem::exists(sources.back()))
            {
                std::cerr << "No se encuentre el archivo " << sources.back() << std::endl;
                return EXIT_FAILURE;
            }
		}
		else if(strcmd.compare("host") == 0)
        {
            host = strparams;
        }
		else if(strcmd.compare("user") == 0)
        {
            user = strparams;user = strparams;
        }
		else if(strcmd.compare("port") == 0)
        {
            port = cave::core::to_number<decltype(port)>(strparams.c_str());
        }
		else if(strcmd.compare("database") == 0)
        {
            database = strparams;
        }
		else if(strcmd.compare("password") == 0)
        {
            if(cmdpwd.compare("[require]") == 0)
            {
                std::cout << "Inserta la contraseña : ";
                std::cin >> password;
            }
            else
            {
                password = strparams;
            }

        }
    }


    cave::mmsql::Data dtm(host, user, password, port);
	bool conectfl = false;
	cave::mmsql::Connection connection;
	try
	{
		conectfl = connection.connect(dtm, false);
	}
	catch (const cave::ExceptionDriver& e)
	{
		std::cout << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}

	if(conectfl)
    {
        cave::mmsql::execute(connection,sources);
        connection.commit();
    }

    return EXIT_SUCCESS;
}

int create(int argc, char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
        //std::cout << "Create : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
        if(strcmp("user",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "Faltan opciones adicionales en create user" << std::endl;
                return EXIT_FAILURE;
            }

            return create_user(argc - i,&argv[i]);
        }
        else if(strcmp("database",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "Faltan opciones adicionales create database" << std::endl;
                return EXIT_FAILURE;
            }

            return create_database(argc - i,&argv[i]);
        }
        else
        {
            std::cerr << "Deve especificarse un sub-comando para create, se encontro '" << argv[i] << "'" << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int create_database(int argc, char* argv[])
{
    std::string user = "root",password,database, host = "localhost";
    int port = 3306;

    for(int i = 1; i < argc; i++)
    {
        //std::cout << "Database : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
        if(strcmp("--password",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para pakage" << std::endl;
                return EXIT_FAILURE;
            }

            password = argv[++i];
        }
        else if(strcmp("--database",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys database" << std::endl;
                return EXIT_FAILURE;
            }
            //std::cout << "\tDatabase : \t\t" << argv[i+1] << std::endl;

            database = argv[++i];
        }
        else if(strcmp("--host",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            host = argv[++i];
        }
        else if(strcmp("--user",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            user = argv[++i];
        }
        else if(strcmp("--port",argv[i]) == 0)
        {
            if(i >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }
            port = std::stoi(argv[++i]);
        }
    }

    cave::mmsql::Data dtm(host, user, password, port);
	bool conectfl = false;
	cave::mmsql::Connection connection;
	try
	{
		conectfl = connection.connect(dtm, true);
	}
	catch (const cave::ExceptionDriver& e)
	{
		std::cout << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
    dtm.print(std::cout);

    if(conectfl)
    {
        if(cave::mmsql::create_database(connection,database)) return EXIT_SUCCESS;
    }

	return EXIT_SUCCESS;
}
int create_user(int argc, char* argv[])
{
    std::string user = "root", password,database, host = "localhost";
    int port = 3306;

    for(int i = 0; i < argc; i++)
    {
        std::cout << "User : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
        if(strcmp("--password",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para pakage" << std::endl;
                return EXIT_FAILURE;
            }

            password = argv[++i];
        }
        else if(strcmp("--database",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys database" << std::endl;
                return EXIT_FAILURE;
            }

            database = argv[++i];
        }
        else if(strcmp("--host",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            host = argv[++i];
        }
        else if(strcmp("--user",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }

            user = argv[++i];
        }
        else if(strcmp("--port",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }
            port = std::stoi(argv[++i]);
        }
    }

    /*cave::mmsql::Data dtm(host, user, password, port);
	bool conectfl = false;
	cave::mmsql::Connection connection;
	try
	{
		conectfl = connection.connect(dtm, true);
	}
	catch (const cave::ExceptionDriver& e)
	{
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}*/

	return EXIT_SUCCESS;
}
