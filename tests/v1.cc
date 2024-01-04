

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

#include "v1.hh"

#include <string>
#include <iostream>
#include <cave/1/base.hh>

int v1_init(void)
{
    bool conectfl;
    try
	{
		conectfl = conn_root.connect(dt_root, true);
	}
	catch (const cave::ExceptionDriver& e)
	{
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return 1;
	}
	catch (const core::exception& e)
	{
		std::cout << "Exception (cave testing) : " << e.what() << "\n";
		return 1;
	}
	catch (...)
	{
	    return 1;
	}
	if(not conectfl) return 1;

	return 0;
}
int v1_clean(void)
{
    conn_root.close();
	return 0;
}

