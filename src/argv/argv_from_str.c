// Copyright 2006 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <stdlib.h>

#include "str.h"
#include "argv.h"

int argv_from_str(char *str, char ** const argv, int max_argc)
{
	int i, argc = 0;
	
	for (i = 0; i < max_argc; i++)
		argv[i] = NULL;
	
	while (*str) {
		if (argc >= max_argc - 1)
			return argc;
		
		while (char_isspace(*str))
			*(str++) = '\0';
		
		if (!*str)
			return argc;
		
		argv[argc] = str;
		argc++;
		
		while (*str && !char_isspace(*str))
			str++;
	}
	
	return argc;
}
