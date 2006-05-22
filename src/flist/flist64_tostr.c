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

#include "flist/flist.h"
#include "printf/printf.h"

int flist64_tostr(const flist64_t list[], uint64_t val, char **str, char delim)
{
	int i, len = 0, idx = 0;
	char *p;
	
	for (i = 0; list[i].key; i++)
		if (list[i].val & val)
			len += _lucid_snprintf(NULL, 0, "%s%c", list[i].key, delim);
	
	p = malloc(len + 1);
	
	if (p == NULL)
		return -1;
	
	bzero(p, len + 1);
	
	for (i = 0; list[i].key; i++)
		if (list[i].val & val)
			idx += _lucid_snprintf(p+idx, len-idx-1, "%s%c", list[i].key, delim);
	
	*str = p;
	
	return 0;
}
