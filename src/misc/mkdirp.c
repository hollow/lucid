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
#include <errno.h>
#include <sys/stat.h>

#include "misc.h"
#include "open.h"
#include "str.h"
#include "strtok.h"

int mkdirp(const char *path, mode_t mode)
{
	int ok = 1;
	struct stat sb;

	if (str_isempty(path) || str_path_isdot(path))
		return errno = EINVAL, -1;

	strtok_t _st, *st = &_st, *p;

	int curdir = open_read(".");

	if (curdir == -1)
		return -1;

	if (!strtok_init_str(st, path, "/", 0))
		return -1;

	strtok_for_each(st, p) {
		if (mkdir(p->token, 0755) == -1) {
			if (errno != EEXIST || stat(p->token, &sb) == -1) {
				ok = 0;
				break;
			}

			if (!S_ISDIR(sb.st_mode)) {
				errno = ENOTDIR;
				ok = 0;
				break;
			}
		}

		if (chdir(p->token) == -1) {
			ok = 0;
			break;
		}
	}

	if (ok && chmod(".", mode) == -1)
		ok = 0;

	fchdir(curdir);
	close(curdir);

	strtok_free(st);
	return ok ? 0 : -1;
}
