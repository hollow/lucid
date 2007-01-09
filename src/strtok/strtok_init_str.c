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

strtok_t *strtok_init_str(strtok_t *st, const char *s, char delim, int empty)
{
	strtok_t *new;
	char *scpy, *cur, *token;
	int len = str_len(s);
	
	INIT_LIST_HEAD(&(st->list));
	
	scpy = cur = token = str_dup(s);
	
	if (!scpy)
		return NULL;
	
	scpy[len] = delim;
	
	do {
		if (*cur == delim) {
			if (!empty && cur == token)
				token = ++cur;
			
			else {
				*cur++ = '\0';
				
				if (!(new = malloc(sizeof(strtok_t))))
					goto free;
				
				if (!(new->token = str_dup(token)))
					goto free;
				
				list_add_tail(&(new->list), &(st->list));
				
				token = cur;
			}
		}
		
		else
			cur++;
	} while(len--);
	
	goto out;
	
free:
	strtok_free(st);
	st = NULL;
	errno = ENOMEM;

out:
	free(scpy);
	
	return st;
}
