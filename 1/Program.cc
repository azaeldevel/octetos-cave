
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
#include <string.h>

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

                password_connection = argv[++i];
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

                user_connection = argv[++i];
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
            else if(strcmp("check",argv[i]) == 0)
            {
                return check(argc - i,&argv[i]);
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






    int Program::loadconfig(const std::filesystem::path& config_file)
    {
        config.open(config_file);
        libconfig::Setting &root = config.getRoot();

        //
        if(host.empty())
        {
            if(root.exists("host"))
            {
                host = (const char*)root.lookup("host");
                if(host.compare(cmd_require) == 0)
                {
                    std::cout << "Indique Host [localhost]: ";
                    std::cin >> host;
                    if(host.compare(".") == 0)
                    {
                        host = "localhost";
                    }
                }
            }
            else
            {
                std::cout << "Deve indicar Host " << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if(user_connection.empty())
        {
            if(root.exists("user"))
            {
                user_connection = (const char*)root.lookup("user");
                if(user_connection.compare(cmd_require) == 0)
                {
                    std::cout << "Indique usuario [root]: ";
                    std::cin >> user_connection;
                    if(user_connection.compare(".") == 0)
                    {
                        user_connection = "root";
                    }
                }
            }
            else
            {
                std::cout << "Deve indicar el usuario " << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if(password_connection.empty())
        {
            if(root.exists("password"))
            {
                password_connection = (const char*)root.lookup("password");
                if(password_connection.compare(cmd_require) == 0)
                {
                    std::cout << "Indique password : ";
                    std::cin >> password_connection;
                }
            }
            else
            {
                std::cout << "Deve indicar el password " << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        if(port == 0)
        {
            std::string strport;
            if(root.exists("port"))
            {
                strport = (const char*)root.lookup("port");
                if(strport.compare(cmd_require) == 0)
                {
                    std::cout << "Indique puerto de conexion a base de datos : ";
                    std::cin >> strport;
                    if(strport.compare(".") == 0)
                    {
                        strport = "3306";
                    }
                    port = std::stoi(strport);
                }
                else
                {
                    port = std::stoi(strport);
                }
                if(port == 0)
                {
                    port = 3306;
                }
            }
            else
            {
                std::cout << "Deve indicar el password " << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        std::cout << "port : " << port << std::endl;


        if(database.empty())
        {
            if(root.exists("database"))
            {
                database = (const char*)root.lookup("database");
                if(database.compare(cmd_require) == 0)
                {
                    std::cout << "Nueva base de datos : ";
                    std::cin >> database;
                }
            }
            else
            {
                std::cout << "Deve indicar la base de datos " << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        //std::cout << "database : " << database << std::endl;
        const libconfig::Setting &list = root["files"];
        //std::cout << "cantidad : " << list.getLength() << std::endl;
        for(int i = 0; i < list.getLength(); i++)
        {
            std::cout << (std::string)list[i] << "\n";
            files.push_back(repodir/(std::filesystem::path)(std::string)list[i]);
        }


        if(templatedb.empty())
        {
            if(root.exists("template"))
            {
                templatedb = (const char*)root.lookup("template");
                if(templatedb.compare("none") == 0)
                {
                    templatedb = "none";
                }
                else if(templatedb.compare(cmd_require) == 0)
                {
                    std::cout << "Indique plantilla [none]: ";
                    std::cin >> templatedb;
                    if(templatedb.compare(".") == 0)
                    {
                        templatedb = "none";
                    }
                }
            }
            else
            {
                templatedb = "none";
            }
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
        if(user_connection.compare(cmd_require) == 0)
        {
            user_connection = "root";
            std::cout << "Conection user [" << user_connection << "] : ";
            std::string cmd;
            std::cin >> cmd;
            if(cmd.compare("S") == 0 or cmd.compare("s") == 0 )
            {

            }
            else
            {
                user_connection = cmd;
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
        if(password_connection.compare(cmd_require) == 0)
        {
            password_connection.clear();
            std::cout << "Conection Password [" << user_connection << "] : ";
            std::cin >> password_connection;
        }

        return EXIT_SUCCESS;
    }

    int Program::repository_import(int argc, char* argv[])
    {
        mmsql::Data dt_create;
        //std::cout << "argv[1] : " << argv[1] << "\n";
        if(argc == 2)
        {
            config_file = argv[1];
            std::cout << "config : " << config_file << "\n";
        }
        else if(argc == 3)
        {
            config_file = argv[1];
            std::cout << "config : " << config_file << "\n";
            config_update = argv[2];
            std::cout << "config-update : " << config_file << "\n";
        }
        /*else if(argc == 4 and strcmp(argv[2],"--update-config") == 0)
        {
            config_file = argv[1];
            std::cout << "config : " << config_file << "\n";
            config_update = argv[2];
            std::cout << "config-update : " << config_file << "\n";
        }*/
        else
        {
            std::cerr << "El sub-comando import tiene los formato los siguinetes : ";
            std::cerr << "\timport 'archivo de repositorio'";
            std::cerr << "\timport 'archivo de repositorio' archivo de configuracion poara actualizar";
            //std::cerr << "\timport 'archivo de repositorio' 'archivo de configuracion poara actualizar' 'path in config file'";
        }

        std::vector<std::string> header;
        //std::cout << file << std::endl;
        if(repodir.empty()) repodir = config_file.parent_path();
        //std::cout << "Repositorio : " << repodir << std::endl;

        loadconfig(config_file);

        //std::cout << "data : " << host << "," <<  user_connection << "," <<  password_connection << "," <<  port  << "," <<  database  << "," <<  templatedb << std::endl;


        mmsql::Data dtm(host, user_connection, password_connection, port);
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

            std::cout << "Executing...\n";

            if(templatedb.compare("none") == 0)
            {
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
            }
            else if(templatedb.compare("muposys-1") == 0)
            {
                std::string newuser,newpass;
#ifdef OCTETOS_CAVE_1_DEVELOPING
                std::cout << "Nuevo usuario [develop]: ";
                std::cin >> newuser;
                if(newuser.compare(".") == 0)
                {
                    newuser = "develop";
                }

                std::cout << "Nueva contrasena [123456]: ";
                std::cin >> newpass;
                if(newpass.compare(".") == 0)
                {
                    newpass = "123456";
                }
#else
                std::cout << "Nuevo usuario : ";
                std::cin >> newuser;

                std::cout << "Nueva contrasena : ";
                std::cin >> newpass;
#endif // OCTETOS_CAVE_1_DEVELOPING
                Script script;
                script.create_user(newuser.c_str(),true,host.c_str(),newpass.c_str());
                script.create_database(database.c_str());
                script.grand_all_privileges(database.c_str(),newuser.c_str(),host.c_str(),newpass.c_str());
                script.use(database.c_str());
                for(const std::filesystem::path& p : files)
                {
                    //std::cout << "Execution " << p.string() << "\n";
                    script.load(p);
                }
                //script.print(std::cout);
                script.execute(connection,true,std::cout);
                if(not config_update.empty()) updateconfig(database,newuser,newpass,host);
            }



        }
        std::cout << "Completada....\n";

        return EXIT_SUCCESS;
    }



    int Program::check(int argc, char* argv[])
    {
        return EXIT_SUCCESS;
    }


    bool Program::updateconfig(const std::string& db,const std::string& u,const std::string& p,const std::string& h)
    {
        libconfig::Setting &root = config.getRoot();
        std::cout << "Updating config...\n";

        root["host"] = h.c_str();
        root["database"] = db.c_str();
        root["user"] = u.c_str();
        root["password"] = p.c_str();


        config.save(config_update);

        return true;
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




