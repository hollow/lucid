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

static inline
int skip_to_newline(int fd)
{
	char c;
	
	while (1) {
		switch(read(fd, &c, 1)) {
		case -1:
			return -1;
		
		case 0:
			return 0;
		
		default:
			if (c != '\n' && c != '\r')
				return lseek(fd, lseek(fd, 0, SEEK_CUR) - 1, SEEK_SET);
		}
	}
}

int str_readline(int fd, char **line)
{
	int chunks = 1, len = 0;
	char *buf = mem_alloc(chunks * CHUNKSIZE + 1);
	char c;

	while (1) {
		switch(read(fd, &c, 1)) {
		case -1:
			mem_free(buf);
			return -1;
		
		case 0:
			goto out;
		
		default:
			if (c == '\n' || c == '\r') {
				if (skip_to_newline(fd) == -1) {
					mem_free(buf);
					return -1;
				}
				
				goto out;
			}
			
			if (len >= chunks * CHUNKSIZE) {
				chunks++;
				buf = mem_realloc(buf, chunks * CHUNKSIZE + 1);
			}
			
			buf[len++] = c;
			break;
		}
	}
	
out:
	*line = buf;
	return len;
}
