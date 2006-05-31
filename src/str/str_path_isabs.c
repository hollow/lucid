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
#include <string.h>

#include "str/str.h"

int str_path_isabs(const char *str)
{
	char *p, *path, *o;
	
	if (str_isempty(str))
		return 0;
	
	if (*str != '/')
		return 0;
	
	if (str_path_isdot(str))
		return 0;
	
	path = o = strdup(str);
	
	for (p = strtok(path, "/"); p; p = strtok(NULL, "/")) {
		if (!str_isgraph(p)) {
			free(o);
			return 0;
		}
	}
	
	free(o);
	return 1;
}
