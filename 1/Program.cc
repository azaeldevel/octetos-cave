
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
#include <core/3/Configuration.hh>

#include "Program.hh"


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
            //std::cout << "Main : \t\t" << argv[i] << "(" << &argv[i] << "):" << argc << std::endl;
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
    bool update_config(const std::filesystem::path& file,const mmsql::Data& dt)
    {
        libconfig::Config config;
        config.readFile(file.string().c_str());

        libconfig::Setting &root = config.getRoot();
        std::cout << "DB : " << (std::string) dt.get_database() << "\n";
        root["database"]["mmsql"]["host"] = dt.get_host();
        root["database"]["mmsql"]["user"] = dt.get_user();
        root["database"]["mmsql"]["port"] = dt.get_port();
        root["database"]["mmsql"]["password"] = dt.get_password();
        root["database"]["mmsql"]["database"] = dt.get_database();

        config.writeFile(file.string().c_str());

        return true;
    }
    bool update_config(core::Configuration& file,const mmsql::Data& dt)
    {
        libconfig::Setting &database_setting = file.lookup("database");
        std::cout << "DB : " << (std::string) dt.get_database() << "\n";
        database_setting["mmsql"]["host"] = dt.get_host();
        database_setting["mmsql"]["user"] = dt.get_user();
        database_setting["mmsql"]["port"] = dt.get_port();
        database_setting["mmsql"]["password"] = dt.get_password();
        database_setting["mmsql"]["database"] = dt.get_database();

        file.save();

        return true;
    }
    int Program::repository_import(int argc, char* argv[])
    {
        std::filesystem::path repodir,file = argv[1];
        std::filesystem::path config_file;
        mmsql::Data dt_create;
        if(argc == 2)
        {
            file = argv[1];
        }
        else if(argc == 4 and strcmp(argv[2],"--update-config") == 0)
        {
            file = argv[1];
            config_file = argv[3];
            //std::cout << "config : " << config_file << "\n";
        }
        else if(argc == 5)
        {
            std::cerr << "En desarrollo...!\n";
        }
        else
        {
            std::cerr << "El sub-comando import tiene los formato los siguinetes : ";
            std::cerr << "\timport 'archivo de repositorio'";
            std::cerr << "\timport 'archivo de repositorio' archivo de configuracion poara actualizar";
            std::cerr << "\timport 'archivo de repositorio' 'archivo de configuracion poara actualizar' 'path in config file'";
        }

        std::vector<std::string> header;
        //std::cout << file << std::endl;
        std::vector<std::filesystem::path> files;
        if(repodir.empty()) repodir = file.parent_path();
        std::cout << "Repositorio : " << repodir << std::endl;
        libconfig::Config config;
        config.readFile(file.string().c_str());
        libconfig::Setting &root = config.getRoot();

        //
        host = (std::string)root.lookup("host");
        //std::cout << "host : " << host << std::endl;
        user = (std::string)root.lookup("user");
        //std::cout << "user : " << user << std::endl;
        password = (std::string)root.lookup("password");
        //std::cout << "password : " << password << std::endl;
        //port = (int)root.lookup("port");
        //std::cout << "port : " << port << std::endl;

        database = (std::string)root.lookup("database");
        //std::cout << "database : " << database << std::endl;
        const libconfig::Setting &list = root["files"];
        //std::cout << "cantidad : " << list.getLength() << std::endl;
        for(int i = 0; i < list.getLength(); i++)
        {
            //std::cout << (std::string)list[i] << "\n";
            files.push_back(repodir/(std::filesystem::path)(std::string)list[i]);
        }

        if(host.compare(cmd_require) == 0)
        {
            host = "localhost";
            std::cout << "Conection Host [" << host << "]: ";
            std::string cmd;
            std::cin >> cmd;
            if(cmd.compare("S") == 0 or cmd.compare("s") == 0 )
            {

            }
            else
            {
                host = cmd;
            }
        }
        if(user.compare(cmd_require) == 0)
        {
            user = "root";
            std::cout << "Conection user [" << user << "] : ";
            std::string cmd;
            std::cin >> cmd;
            if(cmd.compare("S") == 0 or cmd.compare("s") == 0 )
            {

            }
            else
            {
                user = cmd;
            }
        }
        if(port == 0)
        {
            std::cout << "Conection port [" << port << "]: ";
            std::string cmd;
            std::cin >> cmd;
            if(cmd.compare("S") == 0 or cmd.compare("s") == 0 )
            {

            }
            else
            {
                port = std::atoi(cmd.c_str());
            }
        }
        if(password.compare(cmd_require) == 0)
        {
            password.clear();
            std::cout << "Conection Password [" << user << "] : ";
            std::cin >> password;
        }

        if(database.compare("[basic-template]") == 0)
        {
            //
            header = this->resolved_database(database,"database-header",dt_create);
        }

        std::cout << "data : " << host << "," <<  user << "," <<  password << "," <<  port << std::endl;


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
            if(not header.empty())
            {
                for(const std::string& str : header)
                {
                    std::cout << str << "\n";
                    try
                    {
                        connection.execute(str);
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
                }
            }

            std::cout << "Scripting.....\n";

            try
            {
                connection.execute(files);
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

            std::cout << "Config file.....\n";

            //write config file
            if(not config_file.empty())
            {
                if(update_config(config_file,dt_create))
                {
                    connection.commit();
                }
            }
            else
            {
                connection.commit();
            }
        }
        std::cout << "Completada....\n";

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

    std::vector<std::string> Program::resolved_database(const std::string& type,const std::string& name,mmsql::Data& dt)
    {
        std::vector<std::string> sqlheader;
        if(type.compare("[basic-template]") == 0)
        {
            if(name.compare("database-header") == 0)
            {
                std::string database,user,password;
                std::cout << "Database : ";
                std::cin >> database;
#ifdef OCTEOTOS_CAVE_V1_TDD
                user = "develop";
                password = "123456";
#else
                std::cout << "Usuario : ";
                std::cin >> user;
                std::cout << "Password : ";
                std::cin >> password;

#endif // OCTEOTOS_CAVE_V1_TDD

                std::string str = "CREATE  USER IF NOT EXISTS '" + user + "'@'%' IDENTIFIED BY '" + password + "';";
                sqlheader.push_back(str);
                str = "CREATE DATABASE `";
                str += database + "`;";
                sqlheader.push_back(str);
                str = "GRANT ALL PRIVILEGES ON `";
                str += database + "`.* TO '" + user + "'@'%' IDENTIFIED BY '" + password +  "';";
                sqlheader.push_back(str);
                str = "FLUSH PRIVILEGES;";
                sqlheader.push_back(str);
                str = "USE `";
                str += database + "`;";
                sqlheader.push_back(str);

                //verificacion de conexion
                dt.set(host, user, password,database, port);

                return sqlheader;
            }
            else
            {

            }

            return sqlheader;
        }

        return sqlheader;
    }

}




