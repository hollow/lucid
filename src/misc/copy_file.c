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
#include <sys/mman.h>
#include <sys/stat.h>

#include "misc.h"
#include "open.h"

int copy_file(const char *src, const char *dst)
{
	int srcfd = open_read(src);

	if (srcfd == -1)
		return -1;

	int dstfd = open_trunc(dst);

	if (dstfd == -1)
		return -1;

	struct stat sb;

	if (fstat(srcfd, &sb) == -1)
		return -1;

	void *buf = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, srcfd, 0);

	if (buf == MAP_FAILED)
		return -1;

	if (write(dstfd, buf, sb.st_size) != sb.st_size) {
		munmap(buf, sb.st_size);
		return -1;
	}

	munmap(buf, sb.st_size);
	return 0;
}
