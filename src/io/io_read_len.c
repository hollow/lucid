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

#include "io/io.h"

int io_read_len(int fd, char **str, size_t len)
{
	char *buf = malloc(len + 1);
	bzero(buf, len);

	ssize_t buflen = read(fd, buf, len);
	
	if (buflen == -1)
		return -1;
	
	*str = strndup(buf, buflen);
	free(buf);
	return buflen;
}
