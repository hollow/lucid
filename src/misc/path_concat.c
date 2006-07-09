// Copyright 2006 Remo Lemma <coloss7@gmail.com>
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

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "misc/misc.h"
#include "str/str.h"

char *path_concat(char *dirname, char *basename)
{
	char *path;
	
	if (str_path_isdot(basename))
		return errno = EINVAL, NULL;
	
	asprintf(&path, "%s/%s", dirname, basename);
	
	return path;
}
