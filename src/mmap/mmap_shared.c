// Copyright 2005 Felix von Leitner <felix-libowfat@fefe.de>
//           2006 Benedikt Böhm <hollow@gentoo.org>
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

#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

#include "open/open.h"
#include "mmap/mmap.h"

char *mmap_shared(const char *filename, size_t *len)
{
	int fd = open_read(filename);
	char *map;
	
	if (fd >= 0) {
		*len = lseek(fd, 0, SEEK_END);
		
		map = mmap(0, *len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		
		if (map == MAP_FAILED)
			map = NULL;
		
		close(fd);
		
		return map;
	}
	
	return 0;
}
