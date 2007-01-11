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
#include "str.h"

int str_readfile(int fd, char **str)
{
	int chunks = 1, len = 0;
	char *buf = mem_alloc(chunks * CHUNKSIZE + 1);
	
	for (;;) {
		int bytes_read = read(fd, buf+len, CHUNKSIZE);
		
		if (bytes_read == -1) {
			mem_free(buf);
			return -1;
		}
		
		len += bytes_read;
		buf[len] = '\0';
		
		if (bytes_read == 0)
			break;
		
		if (bytes_read == CHUNKSIZE) {
			chunks++;
			buf = mem_realloc(buf, chunks * CHUNKSIZE + 1);
		}
	}
	
	*str = buf;
	return len;
}
