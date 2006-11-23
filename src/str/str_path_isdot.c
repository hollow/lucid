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

int str_path_isdot(const char *str)
{
	char *buf, *p, *o;
	
	if (str_isempty(str))
		return 0;
	
	buf = p = o = str_dup(str);
	
	while (1) {
		p = str_index(p, '/', str_len(p));
		
		if (p) {
			if (o == p) {
				p++;
				continue;
			}
			
			*p++ = '\0';
		}
		
		if (str_cmp(p, ".") == 0 || str_cmp(p, "..") == 0) {
			free(buf);
			return 1;
		}
		
		if (!p)
			break;
		else
			o = p;
	}
	
	free(buf);
	return 1;
}
