
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

namespace oct::cave::v0
{




ExceptionResult::ExceptionResult()
{
}
ExceptionResult::ExceptionResult(const ExceptionResult& e) : core::v3::Exception(e)
{
}

ExceptionResult::ExceptionResult(const char* m) : core::v3::Exception(m)
{
}
ExceptionResult::ExceptionResult(const char* m,const char* fn, unsigned int l) : core::v3::Exception(m,fn,l)
{
}

ExceptionResult::~ExceptionResult()
{
}


ExceptionQuery::ExceptionQuery()
{
}
ExceptionQuery::ExceptionQuery(const ExceptionQuery& e) : core::v3::Exception(e)
{
}

ExceptionQuery::ExceptionQuery(const char* m) : core::v3::Exception(m)
{
}
ExceptionQuery::ExceptionQuery(const char* m,const char* fn, unsigned int l) : core::v3::Exception(m,fn,l)
{
}

ExceptionQuery::~ExceptionQuery()
{
}




DataSource::DataSource(const std::string& d) : database(d)
{
}
const std::string& DataSource::get_database()const
{
	return database;
}

}