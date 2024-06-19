
#ifndef OCTETOS_CAVE_V1_PROGRAM_HH
#define OCTETOS_CAVE_V1_PROGRAM_HH

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

#ifdef OCTEOTOS_CAVE_V1_TDD
	#include <iostream>
#endif



#include <vector>
#include <filesystem>
#include <iostream>
//#include <libconfig.h++>

#include "mmsql.hh"
#include "core/3/Configuration.hh"

namespace oct::cave::v1
{
    class Program
    {
    public:
        int main(int argc, char* argv[]);
        int check(int argc, char* argv[]);
        int loadconfig(const std::filesystem::path&);
        /**
        *\brief Crea un nuevo archivco de configuracion con los paarametros usados para crear la base de datos
        *\param database nombre de la nueva base de datos
        *\param user nombre de la nuevo usuario
        *\param password nombre de la nueva contrasena
        *\param host host en el que se realiza la operacion
        **/
        bool updateconfig(const std::string& database,const std::string& user,const std::string& password,const std::string& host);
        int repository_import(int argc, char* argv[]);
        int repository(int argc, char* argv[]);

        virtual std::vector<std::string> resolved_database(const std::string& type,const std::string& name,mmsql::Data&);

    private:
        std::filesystem::path dir,repodir,config_file,config_update,package = "ocpk";
        std::string user_connection,password_connection,database, host,templatedb;
        std::vector<std::filesystem::path> files;
        int port = 3306;
        core::Configuration config;
    };
}


#endif
