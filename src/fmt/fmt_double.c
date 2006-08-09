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

size_t fmt_double(char *dest, double d, size_t prec)
{
	int i;
	
	union {
		double d;
		unsigned long long x;
	} u = {
		.d = d,
	};
	
	/* sign is stored in the highest bit */
	int sign = u.x >> 63;
	
	/* 12 highest bits except the highest one (0x800, used for sign)
	** need to substract a bias of 1023 due to double has signed exp */
	long exp  = ((u.x >> 52) & ~0x800) - 1023;
	long expa = (exp >= 0) ? exp : -exp - 1;
	
	/* the exponent in memory is base-2, we need base-10 */
	long e10a = 1L + (expa * 0.30102999566398119802); /* log10(2) */
	
	char *dorig = dest;
	
	/* put a negative sign to dest and make d positive
	** so we can catch (d == 0.0) below */
	if (sign) {
		d = -d;
		
		if (dest) *dest = '-';
		dest++;
	}
	
	/* no further processing on null */
	if (d == 0.0) {
		if (dest) *dest = '0';
		dest++;
		
		return dest - dorig;
	}
	
	double pe10 = 10.0;
	
	/* get pow(10, e10a); */
	for (i = e10a; i > 1; i--)
		pe10 = pe10 * 10;
	
	/* get mantissa */
	double m;
	
	if (exp >= 0)
		m = d / pe10;
	else
		m = d * pe10;
	
	/* if precision is greather than 16 strange round errors will occur */
	if (prec > 16)
		prec = 16;
	
	int initial = 1;
	
	/* loop through mantissa */
	while (prec > 0) {
		i  = (int) m;
		
		/* round */
		if (prec == 1 && (m - i) >= 0.6)
			i++;
		
		m -= i;
		
		if (dest) *dest = i + '0';
		dest++;
		
		if (initial) {
			initial = 0;
			if (dest) *dest = '.';
			dest++;
		}
		
		else
			prec--;
		
		m *= 10;
	}
	
	/* use scientific notation if we have a non-null exponent */
	if (e10a != 0) {
		if (dest) *dest = 'e';
		dest++;
		
		dest += fmt_plusminus(dest, exp);
		dest += fmt_ulong(dest, e10a);
	}
	
	return dest - dorig;
}
