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

#include "mem.h"
#include "mem_internal.h"

_mem_pool_t *_mem_pool = 0;

void *mem_alloc(int n)
{
	if (!_mem_pool) {
		if (!(_mem_pool = calloc(1, sizeof(_mem_pool_t))))
			return 0;
		
		INIT_LIST_HEAD(&(_mem_pool->list));
	}
	
	_mem_pool_t *new;
	
	if (!(new = calloc(1, sizeof(_mem_pool_t))))
		return 0;
	
	char *m;
	
	if (!(m = calloc(1, n))) {
		free(new);
		return 0;
	}
	
	m += n;
	
	while (n--)
		*m-- = '\0';
	
	new->mem = m;
	
	list_add_tail(&(new->list), &(_mem_pool->list));
	
	return new->mem;
}
