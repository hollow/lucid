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

#include "str.h"
#include "stralloc.h"
#include "strtok.h"

int strtok_tostr(strtok_t *st, char **str, char *delim)
{
	int i = 0;
	stralloc_t _sa, *sa = &_sa;
	strtok_t *p;

	if (strtok_count(st) < 1)
		return 0;

	stralloc_init(sa);

	strtok_for_each(st, p) {
		if (stralloc_catf(sa, "%s%s", p->token, delim) == -1)
			return -1;

		i++;
	}

	if (sa->len > 0)
		sa->len -= str_len(delim);

	*str = stralloc_finalize(sa);

	stralloc_free(sa);
	return i;
}
