
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


#include <cstring>
#include <fstream>

#include "Program.hh"
#include "mmsql.hh"

#if defined (__linux__) && defined (IDE_CODEBLOCKS)

#elif defined(_WIN32) || defined(_WIN64)

#elif defined (__linux__)

#else
	#error "Plataforma desconocida"
#endif

//namespace cave = oct::cave::v1;
const std::string cmdpwd("[require]");
const std::string cmd_require("[require]");

namespace oct::cave::v1
{
    int Program::main(int argc, char* argv[])
    {
        for(int i = 1; i < argc; i++)
        {
            std::cout << "Main : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
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
            else if(strcmp("--package",argv[i]) == 0)
            {
                if(i >= argc)
                {
                    std::cerr << "No se espesifico el valor para muposys password" << std::endl;
                    return EXIT_FAILURE;
                }

                package = argv[++i];
            }
            else if(strcmp("repository",argv[i]) == 0)
            {
                return repository(argc - i,&argv[i]);
            }
        }
        return EXIT_FAILURE;
    }
    int Program::repository_import(int argc, char* argv[])
    {
        std::filesystem::path file = argv[1];
        std::cout << file << std::endl;
        std::vector<std::filesystem::path> files;
        libconfig::Config config;
        config.readFile(file.c_str());
        libconfig::Setting &root = config.getRoot();

        //
        host = (std::string)root.lookup("host");
        user = (std::string)root.lookup("user");
        password = (std::string)root.lookup("password");
        if(root.exists("port"))
        {
            //port = root.lookup("port");
        }


        //std::cout << "name : " << name<< std::endl;
        //std::cout << "password : " << password << std::endl;
        //database = root.lookup("database");
        //std::cout << "password : " << password << std::endl;

        //std::cout << "port : " << port << std::endl;

        const libconfig::Setting &list = root["files"];
        std::cout << "cantidad : " << list.getLength() << std::endl;
        for(int i = 0; i < list.getLength(); i++)
        {
            std::cout << (std::string)list[i] << "\n";
            files.push_back((std::filesystem::path)list[i]);
        }

        if(host.compare(cmd_require) == 0)
        {
            host.clear();
            std::cout << "Indicar Host : ";
            std::cin >> host;
        }
        if(user.compare(cmd_require) == 0)
        {
            host.clear();
            std::cout << "Indicar user : ";
            std::cin >> user;
        }
        if(port == 0)
        {
            std::cout << "Indicar port : ";
            std::cin >> port;
        }
        if(password.compare(cmd_require) == 0)
        {
            password.clear();
            std::cout << "Inserta la password para " << user << " : ";
            std::cin >> password;
        }

        mmsql::Data dtm(host, user, password, port);
        bool conectfl = false;
        mmsql::Connection connection;
        try
        {
            conectfl = connection.connect(dtm, false);
        }
        catch (const ExceptionDriver& e)
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
            mmsql::execute(connection,files,true);
            connection.commit();
        }

        return EXIT_SUCCESS;
    }
    int Program::repository(int argc, char* argv[])
    {
        for(int i = 1; i < argc; i++)
        {
            //std::cout << "Database : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
            if(strcmp("import",argv[i]) == 0)
            {
                if(i >= argc)
                {
                    std::cerr << "No se espesifico el valor para pakage" << std::endl;
                    return EXIT_FAILURE;
                }

                return repository_import(argc - i,&argv[i]);
            }

        }



        return EXIT_SUCCESS;
    }

}




