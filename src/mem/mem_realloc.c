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

#include "mem.h"
#include "mem_internal.h"

void *mem_realloc(void *s, int n)
{
	_mem_pool_t *p;
	
	if (!s)
		return mem_alloc(n);

	if (n == 0) {
		mem_free(s);
		return NULL;
	}
	
	mem_for_each(_mem_pool, p)
		if (p->mem == s)
			break;
	
	if (p->mem != s) {
		errno = EINVAL;
		return 0;
	}
	
	char *m = realloc(p->mem, n);
	
	if (!m)
		return 0;
	
	p->mem = m;
	
	return p->mem;
}
