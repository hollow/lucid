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
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

#include "misc.h"
#include "printf.h"

int runlink(const char *path)
{
	struct stat sb;
	
	DIR *dp;
	struct dirent *d;
	
	int status = 0;
	char *p, *new_path;
	
	if (lstat(path, &sb) == -1) {
		if (errno == ENOENT)
			return 0;
		else
			return -1;
	}
	
	if (S_ISDIR(sb.st_mode)) {
		if (!(dp = opendir(path)))
			return -1;
		
		while ((d = readdir(dp))) {
			p = d->d_name;
			
			if (p && p[0] == '.' && (!p[1] || (p[1] == '.' && !p[2])))
				continue;
			
			_lucid_asprintf(&new_path, "%s/%s", path, d->d_name);
			
			if (runlink(new_path) == -1)
				status = -1;
			
			free(new_path);
		}
		
		if (closedir(dp) == -1)
			return -1;
		
		if (rmdir(path) == -1)
			return -1;
		
		return status;
	}
	
	if (unlink(path) == -1)
		return -1;
	
	return 0;
}
