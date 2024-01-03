
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
#include <libconfig.h++>

#include "mmsql.hh"

namespace oct::cave::v1
{
    class Program
    {
    public:
        int main(int argc, char* argv[]);
        int repository_import(int argc, char* argv[]);
        int repository(int argc, char* argv[]);

        virtual std::vector<std::string> resolved_database(const std::string& type,const std::string& name,mmsql::Data&);

    private:
        std::filesystem::path dir,package = "ocpk";
        std::string user = "root",password,database, host = "localhost";
        int port = 3306;
    };
}


#endif
