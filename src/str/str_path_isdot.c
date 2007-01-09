// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#include <stdlib.h>

#include "str.h"

int str_path_isdot(const char *str)
{
	char *buf, *p, *o;
	
	if (str_isempty(str))
		return 0;
	
	buf = p = o = str_dup(str);
	
	while (1) {
		p = str_chr(p, '/', str_len(p));
		
		if (p)
			*p++ = '\0';
		
		if (str_cmp(o, ".") == 0 || str_cmp(o, "..") == 0) {
			free(buf);
			return 1;
		}
		
		if (!p)
			break;
		else
			o = p;
	}
	
	free(buf);
	return 0;
}
