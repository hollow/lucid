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

size_t fmt_fill(char *dest, size_t srclen, size_t padlen)
{
	size_t len;
	int i;
	
	char *dorig = dest;
	
	if (!dest)
		return srclen > padlen ? srclen : padlen;
	
	dest += srclen;
	
	for (i = padlen - srclen; i > 0; i--) {
		*dest=' ';
		dest++;
	}
	
	return dest - dorig;
}
