

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

#include "v0.hh"

#include <string>
#include <iostream>
#if  (defined(_WIN32) || defined(_WIN64))
	#include <cave/src/base.hh>
	#include <cave/src/oct-core.hh>
#elif defined __linux__
	#include <src/base.hh>
	#include <src/oct-core.hh>
#else
 	#error "Plataforma desconocida."
#endif


int v0_init(void)
{
	return 0;
}
int v0_clean(void)
{
	return 0;
}


void v0_core_core()
{
	std::cout << "\n";

	double media = oct::core::media<int,double>(1,2,3,4,5,6,7,8,9,0);
	//std::cout << "media2 : " <<  media2 << "\n";
	double desv = oct::core::desv<int,double>(1,2,3,4,5,6,7,8,9,0);
	//std::cout << "desv2 : " <<  desv2 << "\n";

	double media1 = oct::core::media<double>(1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,0.0);
	//std::cout << "media1 : " <<  media1 << "\n";
	double desv1 = oct::core::desv<double>(1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,0.0);
	//std::cout << "desv1 : " <<  desv1 << "\n";

	CU_ASSERT(media - media1 < 0.01);
	CU_ASSERT(desv - desv1 < 0.01);
}
