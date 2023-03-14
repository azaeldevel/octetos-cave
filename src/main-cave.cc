
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

#ifdef __linux__

#elif defined __linux__ && defined IDE_CODEBLOCKS
	#include "mmsql.hh"
#elif defined(_WIN32) || defined(_WIN64)

#else
	#error "Plataforma desconocida"
#endif

const char* cmdbegin = "";
int main(int argc, char* argv[])
{
    std::filesystem::path repository;
    std::string user = "root", password,database, host = "localhost",pakage;
    //int port = 3306;

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
        /*else if(strcmp("--port",argv[i]) == 0)
        {
            if(i + 1 >= argc)
            {
                std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                return EXIT_FAILURE;
            }
            port = std::stoi(argv[++i]);
        }*/
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
    if(password.empty())
    {
        std::cerr << "No se espesifico un contraseña para trabajar" << std::endl;
        return EXIT_FAILURE;
    }

    std::fstream mpk(repository/pakage, std::ios::in);
    std::string strline,strcmd,strparams,cmdsql;
    std::list<std::string> cmdlist;
    std::stringstream sline;
    while(std::getline(mpk,strline))
    {
        sline.clear();
        sline << strline;

        std::getline(sline,strcmd,':');
        std::getline(sline,strparams,':');

        std::cout << strcmd << " : " << strparams << std::endl;
    }

	return EXIT_SUCCESS;
}

