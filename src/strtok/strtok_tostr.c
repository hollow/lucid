// Copyright 2007 Benedikt Böhm <hollow@gentoo.org>
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

#include "str.h"
#include "stralloc.h"
#include "strtok.h"

int strtok_tostr(strtok_t *st, char **str, char delim)
{
	int i = 0;
	stralloc_t buf;
	strtok_t *p;
	
	if (strtok_count(st) < 1)
		return 0;
	
	stralloc_init(&buf);
	
	strtok_for_each(st, p) {
		if (stralloc_catf(&buf, "%s%c", p->token, delim) == -1)
			return -1;
		
		i++;
	}
	
	if (buf.len > 0) {
		buf.s[buf.len - 1] = '\0';
		*str = str_dup(buf.s);
	}
	
	stralloc_free(&buf);
	
	return i;
}
