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
#include <sys/stat.h>

#include "misc.h"
#include "str.h"

int mkdirp(const char *path, mode_t mode)
{
	int errno_orig, rc = 0;
	char *p, *buf, c;
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
	
err:
	rc = -1;
out:
	errno_orig = errno;
	free(buf);
	errno = errno_orig;
	return rc;
}
