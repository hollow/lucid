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

#include <sys/stat.h>

#include "mem.h"
#include "misc.h"
#include "str.h"

int ismount(const char *path)
{
	struct stat sb_path, sb_parent;
	int rc = 1;

	if (str_isempty(path))
		return 0;

	if (str_equal(path, "/"))
		return 1;

	char *parent = str_path_dirname(path);

	if (lstat(path, &sb_path) == -1 ||
			!S_ISDIR(sb_path.st_mode) ||
			lstat(parent, &sb_parent) == -1 ||
			sb_path.st_dev == sb_parent.st_dev)
		rc = 0;

	mem_free(parent);
	return rc;
}
