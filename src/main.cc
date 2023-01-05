
/**
 *  This file is part of Octetos Cave.
 *  Octetos Cave is a database abstract layer.
 *  Copyright (C) 2023  Azael Reyes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */

#include <iostream>
#include <glibmm/i18n.h>

bindtextdomain(GETTEXT_PACKAGE, OCTETOS_CAVE_LOCALEDIR);
bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
textdomain(GETTEXT_PACKAGE);

int main()
{
	std::cout << _("Hello world!") << std::endl;
	return 0;
}

