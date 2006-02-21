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

#include "fmt/fmt.h"

static inline
char tohex(char c)
{
	return c >= 10 ? c - 10 + 'a' : c + '0';
}

size_t fmt_xlong(char *dest, unsigned long i)
{
	size_t len = 1;
	unsigned long tmp = i;
	
	while ((tmp >>= 4) != 0)
		len++;
	
	if (!dest)
		return len;
	
	dest += len;
	
	do {
		*--dest = tohex(i & 15);
	} while ((i >>= 4) != 0);
	
	return len;
}
