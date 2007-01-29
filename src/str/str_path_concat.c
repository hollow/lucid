// Copyright (C) 2006      Remo Lemma <coloss7@gmail.com>
//               2006-2007 Benedikt Böhm <hollow@gentoo.org>
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
#include "printf.h"
#include "str.h"

char *str_path_concat(const char *dirname, const char *basename)
{
	char *path = 0;

	if (str_len(basename) > 1 && mem_cmp(basename, "./", 2) == 0)
		basename += 2;

	if (str_isempty(dirname) || str_path_isdot(basename))
		return 0;

	_lucid_asprintf(&path, "%s/%s", dirname, basename);

	return path;
}
