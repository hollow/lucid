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

void mem_freeall(void)
{
	_mem_pool_t *p, *tmp;
	
	if (!_mem_pool)
		return;
	
	mem_for_each_safe(_mem_pool, p, tmp) {
		list_del(&(p->list));
		munmap(p->mem, p->len);
		munmap(p, sizeof(_mem_pool_t));
	}
}