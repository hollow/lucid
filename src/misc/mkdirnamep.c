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
#include <libgen.h>

#include "misc.h"
#include "str.h"

int mkdirnamep(const char *path, mode_t mode)
{
	char *buf, *dname;
	int rc;
	
	buf = str_dup(path);
	dname = dirname(buf);
	
	rc = mkdirp(dname, mode);
	
	free(buf);
	return rc;
}
