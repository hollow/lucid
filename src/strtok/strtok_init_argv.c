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
#include <errno.h>

#include "str.h"
#include "strtok.h"

strtok_t *strtok_init_argv(strtok_t *st, char *argv[], int argc, int empty)
{
	int i;
	strtok_t *new;
	
	INIT_LIST_HEAD(&(st->list));
	
	for (i = 0; i < argc; i++) {
		if (!empty && str_isempty(argv[i]))
			continue;
		
		if (!(new = malloc(sizeof(strtok_t))))
			goto free;
		
		if (!(new->token = str_dup(argv[i])))
			goto free;
		
		list_add_tail(&(new->list), &(st->list));
	}
	
	goto out;
	
free:
	strtok_free(st);
	st = NULL;
	errno = ENOMEM;
	
out:
	return st;
}
