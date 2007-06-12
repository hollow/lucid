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

#include "mem.h"
#include "sys.h"

#include "mem_internal.h"

static inline
void __small_free(void *_ptr, size_t size)
{
	mem_chunk_t *ptr = CHUNK_START(_ptr);
	size_t       idx = get_index(size);

	/* allways zero out small mem */
	char *p = (char *) ptr;

	while (size--)
		*p++ = 0;

	ptr->next = __small_mem[idx];
	__small_mem[idx] = ptr;
}

/* public functions */
void mem_free(void *ptr)
{
	size_t size;

	if (ptr) {
		size = ((mem_chunk_t *) CHUNK_START(ptr))->size;

		if (size) {
			if (size <= __MAX_SMALL_SIZE)
				__small_free(ptr, size);
			else
				sys_munmap(CHUNK_START(ptr), size);
		}
	}
}
