
#ifndef OCTETOS_CAVE_TESTS_HH
#define OCTETOS_CAVE_TESTS_HH

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

#include <CUnit/Basic.h>


#if  (defined(_WIN32) || defined(_WIN64))

#elif defined __linux__

#else
	#error "Plataforma desconocida."
#endif

#include <cave/1/mmsql.hh>

namespace core = oct::core::v3;
namespace cave = oct::cave::v1;

static cave::mmsql::Data dt_root("tests/config-root");
static cave::mmsql::Data dt_cave("tests/config-cave");

int v1_init(void);
int v1_clean(void);
void v1_develop();
void v1_selects();
void v1_writes();
void v1_script();


#endif
