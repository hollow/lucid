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

#include <unistd.h>
#include <sys/types.h>

/* memory chunks */
typedef struct {
	void *next;
	int   size;
} mem_chunk_t;

#define CHUNK_START(b)  ((void *)(((char *)(b))-sizeof(mem_chunk_t)))
#define CHUNK_RET(b)    ((void *)(((char *)(b))+sizeof(mem_chunk_t)))

#define CHUNK_SIZE      getpagesize()
#define PAGE_ALIGN(s)   (((s)+CHUNK_SIZE-1)&(unsigned long)(~(CHUNK_SIZE-1)))

/* small memory */
extern mem_chunk_t *__small_mem[8];

#define __SMALL_NR(i)		(CHUNK_SIZE/(i))

#define __MIN_SMALL_SIZE	__SMALL_NR(256)		/*   16 /   32 */
#define __MAX_SMALL_SIZE	__SMALL_NR(2)		/* 2048 / 4096 */

#define GET_SIZE(s)		(__MIN_SMALL_SIZE<<get_index((s)))

static inline
size_t get_index(size_t _size)
{
	size_t idx  = 0;
	size_t size = (_size - 1) & (CHUNK_SIZE - 1);
	size >>= (CHUNK_SIZE == 4096) ? 4 : 5;

	while (size) {
		size >>= 1;
		++idx;
	}

	return idx;
}

#endif
