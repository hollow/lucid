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
#include "strtok.h"

int str_path_isabs(const char *str)
{
	int abs = 1;

	if (str_isempty(str))
		return 0;

	if (*str != '/')
		return 0;

	strtok_t _st, *st = &_st, *p;

	if (!strtok_init_str(st, str, "/", 0))
		return -1;

	strtok_for_each(st, p) {
		if (str_equal(p->token, ".") || str_equal(p->token, "..") ||
		    !str_isgraph(p->token)) {
			abs = 0;
			break;
		}
	}

	strtok_free(st);

	return abs;
}
