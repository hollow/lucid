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

#include "str.h"

char *str_path_dirname(const char *path)
{
	/* empty string */
	if (str_isempty(path))
		return str_dup(".");

	/* string consisting entirely of '/' */
	const char *p = path;

	while (*p && *p == '/')
		p++;

	if (!*p)
		return str_dup("/");

	char *dirname = str_dup(path);

	char *q = dirname + str_len(dirname) - 1;

	/* remove trailing '/' */
	while (*q == '/' && q != dirname)
		q--;

	/* remove last portion of the path */
	while (*q != '/' && q != dirname)
		q--;

	/* remove trailing '/' again */
	while (*q == '/' && q != dirname)
		*q-- = '\0';

	return dirname;
}
