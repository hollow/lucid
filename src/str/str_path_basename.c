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

char *str_path_basename(const char *path)
{
	/* empty string */
	if (str_isempty(path))
		return str_dup(".");

	/* string consisting entirely of '/' */
	while (*path && *path == '/')
		path++;

	if (!*path)
		return str_dup("/");

	char *bn, *p, *buf = str_dup(path);

	while ((p = str_rchr(buf, '/', str_len(buf)))) {
		if (p[1] == 0) {
			if (p == buf)
				break;
			else
				*p = 0;
		}

		else {
			bn = str_dup(buf + 1);
			mem_free(buf);
			return bn;
		}
	}

	return buf;
}
