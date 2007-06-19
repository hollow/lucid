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

#ifndef _LUCID_MEM_INTERNAL_H
#define _LUCID_MEM_INTERNAL_H

#include "list.h"

typedef struct {
	list_t list;
	void *mem;
	int len;
} _mem_pool_t;

extern _mem_pool_t *_mem_pool;

#define mem_for_each(pool, p) list_for_each_entry(p, &(pool->list), list)
#define mem_for_each_safe(pool, p, tmp) \
	list_for_each_entry_safe(p, tmp, &(pool->list), list)

#endif
