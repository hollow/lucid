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

#include <errno.h>
#include <sys/mman.h>

#include "mem.h"
#include "mem_internal.h"

void *mem_realloc(void *s, int n)
{
	int len = n;
	_mem_pool_t *p;
	
	if (!s)
		return mem_alloc(n);
	
	mem_for_each(_mem_pool, p)
		if (p->mem == s)
			break;
	
	errno = EINVAL;
	
	if (p->mem != s)
		return 0;
	
	char *m = mremap(p->mem, p->len, len, MREMAP_MAYMOVE);
	
	if (m == MAP_FAILED)
		return 0;
	
	p->mem = m;
	
	m += n;
	
	while (n-- > p->len)
		*m-- = '\0';
	
	p->len = len;
	
	return p->mem;
}
