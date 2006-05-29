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

#include <errno.h>
#include <sys/stat.h>

#include "misc/misc.h"

int mkdirp(char *path, mode_t mode)
{
	char *s = path;
	char c;
	struct stat sb;
	
	do {
		c = 0;
		
		while (*s) {
			if (*s == '/') {
				do { ++s; } while (*s == '/');
				c  = *s;
				*s = '\0';
				break;
			}
			
			else
				++s;
		}
		
		if (mkdir(path, 0777) == -1) {
			if (errno != EEXIST || stat(path, &sb) == -1)
				return -1;
			
			if (!S_ISDIR(sb.st_mode))
				return errno = ENOTDIR, -1;
			
			if (!c)
				return 0;
		}
		
		if (!c) {
			if (chmod(path, mode) == -1)
				return -1;
			
			return 0;
		}
		
		*s = c;
	} while (1);
	
	return -1;
}
