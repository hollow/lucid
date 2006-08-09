// Copyright 2006 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "io.h"

int io_read_eof(int fd, char **file)
{
	int rc = -1;
	size_t chunks = 1, len = 0;
	char *buf = malloc(chunks * CHUNKSIZE + 1);

	for (;;) {
		int bytes_read = read(fd, buf+len, CHUNKSIZE);
		
		if (bytes_read == -1)
			goto err;
		
		len += bytes_read;
		buf[len] = '\0';
		
		if (bytes_read == 0)
			goto out;
		
		if (bytes_read == CHUNKSIZE) {
			chunks++;
			buf = realloc(buf, chunks * CHUNKSIZE + 1);
		}
	}
	
out:
	*file = strndup(buf, len);
	rc = strlen(*file);
err:
	free(buf);
	return rc;
}
