// Copyright (C) 2007 Benedikt Böhm <hollow@gentoo.org>
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
#include <fcntl.h>

#include "misc.h"
#include "str.h"

int copy_file(const char *src, const char *dst)
{
	char *srcdname = str_path_dirname(src);
	char *srcbname = str_path_basename(src);
	char *dstdname = str_path_dirname(dst);
	char *dstbname = str_path_basename(dst);

	int srcdirfd = open(srcdname, O_RDONLY|O_DIRECTORY);

	if (srcdirfd == -1)
		return -1;

	int dstdirfd = open(dstdname, O_RDONLY|O_DIRECTORY);

	if (dstdirfd == -1)
		return -1;

	int rc = copy_fileat(srcdirfd, srcbname, dstdirfd, dstbname);

	mem_free(dstbname);
	mem_free(dstdname);
	mem_free(srcbname);
	mem_free(srcdname);

	return rc;
}
