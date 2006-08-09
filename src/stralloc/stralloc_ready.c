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

#include <stdlib.h>

#include "stralloc.h"

int stralloc_ready(stralloc_t *sa, size_t len)
{
	size_t wanted = len + (len >> 3) + 30;
	char *tmp;
	
	if (!sa->s || sa->a < len) {
		if (!(tmp = realloc(sa->s, wanted)))
			return -1;
		
		sa->a = wanted;
		sa->s = tmp;
	}
	
	return 0;
}
