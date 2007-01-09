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
#include <unistd.h>
#include <errno.h>

#include "str.h"

int str_read(int fd, char **str, int len)
{
	int errno_orig;
	char *buf = calloc(len + 1, sizeof(char));
	
	int buflen = read(fd, buf, len);
	
	if (buflen == -1) {
		errno_orig = errno;
		free(buf);
		errno = errno_orig;
		return -1;
	}
	
	if (buflen > 0)
		*str = str_dup(buf);
	
	free(buf);
	return buflen;
}
