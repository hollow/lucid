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

#include "mem.h"
#include "str.h"

char *str_path_dirname(const char *path)
{
	/* empty string or '..' */
	if (str_isempty(path) || str_equal(path, ".."))
		return str_dup(".");

	/* skip prefixing '/' but preserve exactly one */
	while (*path && *(path+1) && *path == '/' && *(path+1) == '/')
		path++;

	int found = 0;
	char *p, *buf = str_dup(path);

	while ((p = str_rchr(buf, '/', str_len(buf)))) {
		/* remove trailing slash */
		if (p[1] == 0 && p != buf)
			*p = 0;

		/* no basename was found until yet */
		else if (!found) {
			*p = 0;
			found = 1;
		}

		/* a basename was found and no trailing slash anymore */
		else
			break;
	}

	char *dn;

	/* path consists only of basename and slashes */
	if (str_isempty(buf))
		dn = str_dup("/");

	/* path is relative or absolute, basename was stripped */
	else if (p)
		dn = str_dup(buf);

	/* path is relative, no basename was stripped */
	else
		dn = str_dup(".");

	mem_free(buf);

	return dn;
}
