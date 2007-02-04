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

#include "mem.h"
#include "str.h"
#include "strtok.h"

void strtok_delete(strtok_t *st, const char *token)
{
	strtok_t *p;
	list_t *pos, *tmp;

	list_for_each_safe(pos, tmp, &(st->list)) {
		p = list_entry(pos, strtok_t, list);

		if (str_equal(p->token, token)) {
			list_del(pos);
			mem_free(p->token);
			mem_free(p);
		}
	}
}
