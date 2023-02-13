
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


ExceptionDriver::ExceptionDriver(const std::string& m, const std::source_location& l) noexcept : oct::core::v3::exception(m,l), handle(NULL)
{
}

const char* ExceptionDriver::what() const noexcept
{
	if (description.empty())
	{
		description = _location.file_name();
		description += ":";
		description += std::to_string(_location.line()) + " ";
		if (_code_flag) description += "- " + std::to_string(_code);
		if (not _message.empty()) description += _message + " ";
		if (handle) description += driver_message(handle);
	}

	return description.c_str();
}

DataSource::DataSource(const std::string& d) : database(d)
{
}
const std::string& DataSource::get_database()const
{
	return database;
}

}
