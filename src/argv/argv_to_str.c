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

#include <string.h>

#include "argv/argv.h"
#include "stralloc/stralloc.h"

char *argv_to_str(int argc, const char ** const argv)
{
	int i;
	size_t len;
	char *str;
	stralloc_t buf;
	
	stralloc_init(&buf);
	
	for (i = 0; i < argc; i++)
		stralloc_catm(&buf, argv[i], " ", NULL);
	
	if (buf.len < 1)
		len = 0;
	else
		len = buf.len - 1;
	
	str = strndup(buf.s, len);
	stralloc_free(&buf);
	return str;
}
