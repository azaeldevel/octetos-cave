#ifndef OCTETOS_COVE_MARIA_V0_HH
#define OCTETOS_COVE_MARIA_V0_HH


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
 
 
#include "base.hh"
#include "mmsql.hh"

namespace oct::cave::v0::maria
{
	typedef cave::v0::DataMMSQL Data;
	typedef cave::v0::Connection<Data> Connection;
	typedef cave::v0::Result<Data> Result;
}

#endif