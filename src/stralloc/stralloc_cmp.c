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

#include "stralloc.h"

int stralloc_cmp(const stralloc_t *a, const stralloc_t *b)
{
	size_t i, j;
	
	for (i = 0;; ++i) {
		if (i == a->len)
			return i == b->len ? 0 : -1;
		
		if (i == b->len)
			return 1;
		
		if ((j = ((unsigned char)(a->s[i]) - (unsigned char)(b->s[i]))))
			return j;
	}
	
	return j;
}
