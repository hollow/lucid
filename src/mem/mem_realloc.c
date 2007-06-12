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

#ifndef MREMAP_MAYMOVE
#define MREMAP_MAYMOVE     1
#endif

void *mem_realloc(void *ptr, int size)
{
	if (!ptr)
		return mem_alloc(size);

	if (!size) {
		mem_free(ptr);
		return (void *)0;
	}

	mem_chunk_t *chunk = CHUNK_START(ptr);
	size_t need = size + sizeof(mem_chunk_t);

	if (need < sizeof(mem_chunk_t))
		return errno = ENOMEM, (void *)0;

	need = (need <= __MAX_SMALL_SIZE) ? GET_SIZE(need) : PAGE_ALIGN(need);

	if (chunk->size != need) {
		if ((chunk->size <= __MAX_SMALL_SIZE)) {
			void *new = mem_alloc(size);

			if (new) {
				if (size > chunk->size)
					size = chunk->size;

				if (size) {
					char *a = new, *b = ptr;
					size -= sizeof(mem_chunk_t);

					while (size--)
						*a++ = *b++;
				}

				mem_free(ptr);
			}

			return new;
		}

		else {
			size  = PAGE_ALIGN(size);
			chunk = sys_mremap(chunk, chunk->size, size, MREMAP_MAYMOVE);

			if (chunk != MAP_FAILED) {
				chunk->size = size;
				return CHUNK_RET(chunk);
			}
		}
	}

	else
		return ptr;

	return errno = ENOMEM, (void *)0;
}
