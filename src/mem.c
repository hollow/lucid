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

void *mem_alloc(int n)
{
	char *s = malloc(n);

	if (s)
		mem_set(s, 0, n);

	return s;
}

void *mem_ccpy(void *s1, const void *s2, int c, int n)
{
	unsigned char       *a = s1;
	const unsigned char *b = s2;

	while (n--) {
		*a++ = *b;

		if (*b == c)
			return (void *) a;

		b++;
	}

	return 0;
}

void *mem_chr(const void *s, int c, int n)
{
	const unsigned char *p = s;

	for (; n--; p++)
		if (*p == c)
			return (void *) p;

	return 0;
}

int mem_cmp(const void *s1, const void *s2, int n)
{
	int d, i;
	const unsigned char *a = s1;
	const unsigned char *b = s2;

	for (i = 0; i < n; i++)
		if ((d = a[i] - b[i]) != 0)
			return d;

	return 0;
}

void *mem_cpy(void *s1, const void *s2, int n)
{
	unsigned char       *a = s1;
	const unsigned char *b = s2;

	while (n--)
		*a++ = *b++;

	return s1;
}

void *mem_dup(const void *s, int n)
{
	void *d = mem_alloc(n);

	if (d)
		return mem_cpy(d, s, n);

	return 0;
}

void mem_free(void *ptr)
{
	int errno_save = errno;
	free(ptr);
	errno = errno_save;
}

int mem_idx(const void *s, int c, int n)
{
	int i;
	const unsigned char *p = s;

	for (i = 0; n--, *p++; i++)
		if (c == *p)
			return i;

	return -1;
}

void *mem_realloc(void *ptr, int size)
{
	return realloc(ptr, size);
}

void *mem_set(void *s, int c, int n)
{
	unsigned char *p = s;

	while (n--)
		*p++ = c;

	return s;
}
