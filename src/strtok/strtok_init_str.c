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

strtok_t *strtok_init_str(strtok_t *st, const char *str, char *delim, int empty)
{
	strtok_t *new;
	char *scpy, *cur, *token;

	INIT_LIST_HEAD(&(st->list));

	if (!str)
		return st;

	scpy = cur = token = str_dup(str);

	if (!scpy)
		return 0;

	while (token) {
		cur = str_str(cur, delim);

		if (cur) {
			mem_set(cur, 0, str_len(delim));
			cur += str_len(delim);
		}

		if (empty || !str_isempty(token)) {
			if (!(new = mem_alloc(sizeof(strtok_t))))
				goto free;

			if (!(new->token = str_dup(token)))
				goto free;

			list_add_tail(&(new->list), &(st->list));
		}

		token = cur;
	}

	goto out;

free:
	strtok_free(st);
	st = 0;

out:
	mem_free(scpy);
	return st;
}
