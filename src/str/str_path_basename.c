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
#include "strtok.h"

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

	strtok_t _st, *st = &_st;
	strtok_init_str(st, path, "/", 0);

	strtok_prev(&st);

	char *basename = str_dup(st->token);

	strtok_free(st);
	return basename;
}