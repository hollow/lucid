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

#include <stdlib.h>
#include <errno.h>

#include "str.h"
#include "strtok.h"

int strtok_append(strtok_t *st, const char *token)
{
	strtok_t *new;
	
	if (!(new = malloc(sizeof(strtok_t))))
		return -1;
	
	if (!(new->token = str_dup(token))) {
		free(new);
		errno = ENOMEM;
		return -1;
	}
	
	list_add_tail(&(new->list), &(st->list));
	
	return 0;
}
