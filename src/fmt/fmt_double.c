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

#include "fmt.h"

#define EXP_MAX   ((1 << 11) - 1)

size_t fmt_double(char *dest, double d, size_t prec)
{
	int len = 0;
	
	double f;
	unsigned long long i;
	
	union {
		double d;
		unsigned long long x;
	} u = { .d = d };
	
	unsigned long long s = (u.x & 0x8000000000000000) >> 63; // sign
	unsigned long long e = (u.x & 0x7FF0000000000000) >> 52; // exponent
	unsigned long long m = (u.x & 0x000FFFFFFFFFFFFF);       // mantissa
	
	/* not a number */
	if (e == EXP_MAX && m != 0)
		return fmt_str(dest, "nan");
	
	/* pos. infinity */
	else if (e == EXP_MAX && m == 0 && s == 0)
		return fmt_str(dest, "+inf");
	
	/* neg. infinity */
	else if (e == EXP_MAX && m == 0 && s == 1)
		return fmt_str(dest, "-inf");
	
	/* zero */
	else if (e == 0 && m == 0) {
		if (s == 1) {
			if (dest) *dest++ = '-';
			len++;
		}
		
		if (dest) *dest++ = '0';
		len++;
		
		if (dest) *dest++ = '.';
		len++;
		
		while (--prec) {
			if (dest) *dest++ = '0';
			len++;
		}
	}
	
	else {
		if (s == 1) {
			if (dest) *dest++ = '-';
			len++;
			
			d = -d;
		}
		
		/* TODO: do not cast floats! ugly! */
		i = fmt_ulonglong(dest, (unsigned long long) d);
		
		if (dest)
			dest += i;
		
		len += i;
		
		if (prec > 0) {
			if (dest) *dest++ = '.';
			len++;
		}
		
		/* TODO: do not cast floats! ugly! */
		f = (d - (unsigned long long) d) * 10;
		
		for (; prec > 0; prec--, f = (f - i) * 10) {
			/* TODO: do not cast floats! ugly! */
			i = (unsigned long long) f;
			
			/* round */
			if (prec == 1 && (f - i) >= 0.5)
				i++;
			
			if (dest) *dest++ = i + '0';
			len++;
		}
	}
	
	return len;
}
