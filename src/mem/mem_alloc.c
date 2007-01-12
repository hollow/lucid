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

#include <sys/mman.h>

#include "mem.h"
#include "mem_internal.h"

#ifdef MAP_ANONYMOUS
#define MAP(n) mmap(0, n, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0)
#else
#define MAP(n) mmap(0, n, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0)
#endif

_mem_pool_t *_mem_pool = 0;

void *mem_alloc(int n)
{
	if (!_mem_pool) {
		if ((_mem_pool = MAP(sizeof(_mem_pool_t))) == MAP_FAILED)
			return 0;
		
		INIT_LIST_HEAD(&(_mem_pool->list));
	}
	
	_mem_pool_t *new;
	
	if ((new = MAP(sizeof(_mem_pool_t))) == MAP_FAILED)
		return 0;
	
	char *m;
	
	if ((m = MAP(n)) == MAP_FAILED) {
		munmap(new, sizeof(_mem_pool_t));
		return 0;
	}
	
	new->len = n;
	
	m += n;
	
	while (n--)
		*m-- = '\0';
	
	new->mem = m;
	
	list_add_tail(&(new->list), &(_mem_pool->list));
	
	return new->mem;
}
