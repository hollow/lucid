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
#include <sys/types.h>
#include <sys/mman.h>

#include "mem.h"

#include "mem_internal.h"

mem_chunk_t *__small_mem[8];

static inline
void *do_mmap(size_t size)
{
	return mmap(0, size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
}

static inline
void *__small_malloc(size_t n)
{
	mem_chunk_t *ptr;
	int idx;

	idx = get_index(n);
	ptr = __small_mem[idx];

	/* no free blocks */
	if (ptr == 0) {
		ptr = do_mmap(CHUNK_SIZE);

		if (ptr == MAP_FAILED)
			return MAP_FAILED;

		__small_mem[idx] = ptr;

		int i, nr = __SMALL_NR(n) - 1;

		for (i = 0; i < nr ; i++) {
			ptr->next = (void *)(((char *) ptr) + n);
			ptr       = ptr->next;
		}

		ptr->next = 0;

		ptr = __small_mem[idx];
	}

	/* get a free block */
	__small_mem[idx] = ptr->next;
	ptr->next = 0;

	return ptr;
}

void *mem_alloc(int n)
{
	mem_chunk_t *ptr;
	size_t need;

	if (!n)
		return errno = ENOMEM, (void *)0;

	n += sizeof(mem_chunk_t);

	if (n < sizeof(mem_chunk_t))
		return errno = ENOMEM, (void *)0;

	if (n <= __MAX_SMALL_SIZE) {
		need = GET_SIZE(n);
		ptr = __small_malloc(need);
	}

	else {
		need = PAGE_ALIGN(n);

		if (!need)
			ptr = MAP_FAILED;
		else
			ptr = do_mmap(need);
	}

	if (ptr == MAP_FAILED)
		return errno = ENOMEM, (void *)0;

	ptr->size = need;

	return CHUNK_RET(ptr);
}
