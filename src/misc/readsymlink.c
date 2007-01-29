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

#include <unistd.h>

#include "mem.h"
#include "misc.h"

#define CHUNKSIZE 128

char *readsymlink(const char *path)
{
	int chunks = 1, len = 0;
	char *buf = mem_alloc(chunks * CHUNKSIZE + 1);

	for (;;) {
		len = readlink(path, buf, chunks * CHUNKSIZE);

		if (len == -1) {
			mem_free(buf);
			return NULL;
		}

		if (len >= chunks * CHUNKSIZE) {
			chunks++;
			buf = mem_realloc(buf, chunks * CHUNKSIZE + 1);
		}
	}

	buf[len] = '\0';

	return buf;
}
