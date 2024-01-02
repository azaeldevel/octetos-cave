
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

#include "Program.hh"

#if defined (__linux__) && defined (IDE_CODEBLOCKS)

#elif defined(_WIN32) || defined(_WIN64)

#elif defined (__linux__)

#else
	#error "Plataforma desconocida"
#endif

namespace cave = oct::cave::v1;

const std::string cmdpwd("[require]");




int main(int argc, char* argv[])
{
    cave::Program prg;

	return prg.main(argc,argv);
}



