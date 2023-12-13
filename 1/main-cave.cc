
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


bool execute(cave::mmsql::Connection& connection,const std::filesystem::path& source)
{
	if(not std::filesystem::exists(source))
	{
		std::cout << "No se encontro el archivo : " << source << "\n";
		return false;
	}

	cave::Result<char, cave::mmsql::Data> rest;
	std::fstream script(source, std::ios::in);
    std::string strline;
    std::stringstream sline;
    while(std::getline(script,strline))
    {
    	if(strline.empty()) continue;
    	if(strline[0] == '-' and strline[1] == '-') continue;
    	strline.erase(remove(strline.begin(), strline.end(), '\n'), strline.end());
    	strline.erase(remove(strline.begin(), strline.end(), ';'), strline.end());

		std::cout << "'" << strline << "' ..." << "\n";

		try
		{
			//std::cout << "execute Step 1\n";
			rest = connection.execute(strline);
			//std::cout << "execute Step 2\n";
		}
		catch (const cave::ExceptionDriver& e)
		{
			std::cout << "Error : " << e.what() << "\n";
		}
		catch (...)
		{
			std::cout << "Error desconocido...\n";
		}

		rest.close();
    }

	return false;
}

int create(int argc, char* argv[]);
int create_database(int argc, char* argv[]);
int create_user(int argc, char* argv[]);
int import(int argc, char* argv[]);

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
int import(int argc, char* argv[])
{
    std::filesystem::path repository;
    std::string user = "root", password,database, host = "localhost",pakage;
    int port = 3306;

    for(int i = 0; i < argc; i++)
    {
        if(strcmp("-r",argv[i]) == 0 or strcmp("--repository",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para repository" << std::endl;
                return EXIT_FAILURE;
            }

            repository = argv[++i];
        }
        else if(strcmp("--package",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para pakage" << std::endl;
                return EXIT_FAILURE;
            }

            pakage = argv[++i];
        }
        else if(strcmp("--password",argv[i]) == 0)
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


    if(repository.empty())
    {
        std::cerr << "No se espesifico un reposistorio para trabajar" << std::endl;
        return EXIT_FAILURE;
    }
    if(pakage.empty())
    {
        std::cerr << "No se espesifico un paquete para trabajar" << std::endl;
        return EXIT_FAILURE;
    }



    cave::mmsql::Data dtm(host, user, password, database, port);
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
	}

	if(not conectfl)
	{
		std::cout << "Fallo la conexion a la base de tatos.\n";
		return EXIT_FAILURE;
	}
	if(not connection.is_connected())
	{
		std::cout << "Fallo la conexion a la base de tatos.\n";
		return EXIT_FAILURE;
	}

    std::fstream mpk(repository/pakage, std::ios::in);
    std::string strline,strcmd,strparams;
    std::stringstream sline;
    bool flexe;
    while(std::getline(mpk,strline))
    {
        sline.clear();
        sline << strline;

        std::getline(sline,strcmd,':');
        std::getline(sline,strparams,':');

        //std::cout << strcmd << " : " << strparams << std::endl;
        if(strcmd.compare("source") == 0)
		{
			flexe = execute(connection,repository/strparams);
			if(flexe) break;
		}
    }
}
