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

#include "char.h"
#include "str.h"

static inline
int char_todigit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	
	else if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	
	else
		return -1;
}

int str_toumax(const char *str, unsigned long long int *val, int base, int n)
{
	char c;
	const char *p = str;
	int d, minus = 0;
	unsigned long long int v = 0;
	
	while (n && char_isspace((unsigned char) *p)) {
		p++;
		n--;
	}
	
	/* Single optional + or - */
	if (n) {
		c = *p;
		
		if (c == '-' || c == '+') {
			minus = (c == '-');
			p++;
			n--;
		}
	}
	
	if (base == 0) {
		if (n >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
			n -= 2;
			p += 2;
			base = 16;
		}
		
		else if (n >= 1 && p[0] == '0') {
			n--;
			p++;
			base = 8;
		}
		
		else {
			base = 10;
		}
	}
	
	else if (base == 16) {
		if (n >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
			n -= 2;
			p += 2;
		}
	}
	
	while (n && (d = char_todigit(*p)) >= 0 && d < base) {
		v = v * base + d;
		n--;
		p++;
	}
	
	if (p - str > 0)
		*val = minus ? -v : v;
	
	return p - str;
}
