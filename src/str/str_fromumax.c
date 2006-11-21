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

#include "str.h"

static inline
char char_fromdigit(int d)
{
	if (d >= 0 && d <= 9)
		return d + '0';
	
	else if (d >= 10 && d <= 36)
		return d + 'A' - 10;
	
	else
		return -1;
}

int str_fromumax(unsigned long long int i, char *str, int base)
{
	int len = 1;
	unsigned long long int tmp = i;
	char *p = str;
	
	while ((tmp /= base))
		len++;
	
	if (!str)
		return len;
	
	p += len;
	
	do {
		*--p = char_fromdigit((i % base));
	} while ((i /= base));
	
	return len;
}
