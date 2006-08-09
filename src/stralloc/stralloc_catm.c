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

#include <stdarg.h>

#include "stralloc.h"

int stralloc_catm(stralloc_t *dst, ...)
{
	va_list ap;
	char *s;
	
	va_start(ap, dst);
	
	while ((s = va_arg(ap, char *))) {
		if (stralloc_cats(dst, s) == -1) {
			va_end(ap);
			return -1;
		}
	}
	
	va_end(ap);
	return 0;
}
