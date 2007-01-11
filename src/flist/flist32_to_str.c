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

#include "flist.h"
#include "str.h"
#include "stralloc.h"

char *flist32_to_str(const flist32_t list[], uint32_t val, char *delim)
{
	int i;
	char *buf;
	stralloc_t _sa, *sa = &_sa;
	
	stralloc_init(sa);
	
	for (i = 0; list[i].key; i++)
		if (val & list[i].val)
			stralloc_catf(sa, "%s%s", list[i].key, delim);
	
	if (sa->len > 0)
		sa->len -= str_len(delim);
	
	buf = stralloc_finalize(sa);
	
	stralloc_free(sa);
	return buf;
}
