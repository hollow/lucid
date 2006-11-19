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
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "misc.h"
#include "str.h"

int mkdirp(const char *path, mode_t mode)
{
	char *p, *buf;
	char c;
	struct stat sb;
	
	if (str_isempty(path))
		return errno = EINVAL, -1;
	
	buf = p = str_dup(path);
	
	do {
		c = 0;
		
		while (*p) {
			if (*p == '/') {
				do { ++p; } while (*p == '/');
				c  = *p;
				*p = '\0';
				break;
			}
			
			else
				++p;
		}
		
		if (mkdir(buf, 0777) == -1) {
			if (errno != EEXIST || stat(buf, &sb) == -1)
				goto err;
			
			if (!S_ISDIR(sb.st_mode)) {
				errno = ENOTDIR;
				goto err;
			}
			
			if (!c)
				goto out;
		}
		
		if (!c) {
			if (chmod(buf, mode) == -1)
				goto err;
			
			goto out;
		}
		
		*p = c;
	} while (1);
	
	return -1;
	
out:
	free(buf);
	return 0;
err:
	free(buf);
	return -1;
}
