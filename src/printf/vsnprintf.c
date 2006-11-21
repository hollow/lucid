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

#include <stdlib.h>

#include "fmt.h"
#include "printf.h"
#include "str.h"

/* format structs */
typedef struct {
	int alt;
	int zero;
	int left;
	int blank;
	int sign;
} __flags_t;

typedef struct {
	int isset;
	int width;
} __prec_t;

typedef struct {
	__flags_t f;
	__prec_t  p;
	int w;
	int l;
} __printf_t;

/* supported formats:
** - format flags: #, 0, -, ' ', +
** - field width
** - argument precision
** - length mods: hh, h, l, and ll
** - conversion spec: d, i, u, o, x, f, c, s, p */
int _lucid_vsnprintf(char *str, size_t size, const char *fmt, va_list _ap)
{
	/* keep track of string length */
	size_t idx = 0;
	
	int l, i;
	char c;
	const char *fmtp = fmt;
	
	va_list ap;
	va_copy(ap, _ap);
	
	/* sanitize destination buffer */
	for (i = 0; i < size; i++)
		*(str+i) = 0;
	
	while ((c = *fmt++)) {
		if (c != '%') {
			if (idx < size)
				str[idx] = c;
			
			idx++;
		}
		
		else {
			/* format struct */
			__printf_t f;
			
			/* sanitize struct with default values */
			f.w = 0;
			f.l = 0;
			
			f.f.alt   = 0;
			f.f.zero  = 0;
			f.f.left  = 0;
			f.f.blank = 0;
			f.f.sign  = 0;
			
			f.p.isset = 0;
			f.p.width = 1;
			
			
			/* parse flags */
			while ((c = *fmt++)) {
				if      (c == '#') f.f.alt   = 1;
				else if (c == '0') f.f.zero  = 1;
				else if (c == '-') f.f.left  = 1;
				else if (c == ' ') f.f.blank = 1;
				else if (c == '+') f.f.sign  = 1;
				else               break;
			}
			
			/* parse field width */
			if (c == '*') {
				l = va_arg(ap, int);
				
				if (l >= 0)
					f.w = l;
				else {
					f.w = -l;
					f.f.left = 1;
				}
				
				c = *fmt++;
			}
			
			else if (char_isdigit(c)) {
				do {
					f.w = f.w * 10 + c - '0';
					c = *fmt++;
				} while (char_isdigit(c));
			}
			
			/* parse precision */
			if (c == '.') {
				c = *fmt++;
				
				f.p.isset = 1;
				f.p.width = 0;
				
				if (c == '*') {
					l = va_arg(ap, int);
					
					if (l >= 0)
						f.p.width = l;
					
					c = *fmt++;
				}
				
				else if (char_isdigit(c)) {
					do {
						f.p.width = f.p.width * 10 + c - '0';
						c = *fmt++;
					} while (char_isdigit(c));
				}
			}
			
			/* parse length modifier */
			f.l = 2;
			
			if (c == 'l') {
				f.l = 3;
				c = *fmt++;
				
				if (c == 'l') {
					f.l = 4;
					c = *fmt++;
				}
			}
			
			else if (c == 'h') {
				f.l = 1;
				c = *fmt++;
				
				if (c == 'h') {
					f.l = 0;
					c = *fmt++;
				}
			}
			
			/* sign overrides blank */
			if (f.f.sign)
				f.f.blank = 0;
			
			/* left overrides zero */
			if (f.f.left)
				f.f.zero = 0;
			
			/* no zero padding if precision is specified */
			if (f.p.isset && f.w > 0)
				f.f.zero = 0;
			
			/* buffer for alternate notation string */
			char *alt;
			
			/* signed arguments */
			signed long long int darg;
			
			/* unsigned long argument */
			unsigned long long int uarg;
			
			/* unsigned conversion function */
			int (*ufn)(char *, unsigned long long int);
			
			/* float argument */
			double farg;
			
			/* character argument */
			int carg;
			
			/* string argument */
			const char *sarg;
			
			/* pointer argument */
			void *parg;
			
			/* number argument */
			int *narg;
			
			/* used for precision counting */
			int len;
			
			/* temporary buffer for integer conversions */
			char ibuf[64];
			
			/* temporary buffer for string conversion */
			const char *buf = ibuf;
			
			/* buffer length */
			size_t buflen = 0;
			
			/* do conversions */
			switch (c) {
				case 'd':
				case 'i': /* signed conversion */
					switch (f.l) {
					case 0:
						darg = (signed char) va_arg(ap, signed int);
						break;
					
					case 1:
						darg = (signed short int) va_arg(ap, signed int);
						break;
					
					case 2:
						darg = (signed int) va_arg(ap, signed int);
						break;
					
					case 3:
						darg = (signed long int) va_arg(ap, signed long int);
						break;
					
					case 4:
						darg = (signed long long int) va_arg(ap, signed long long int);
						break;
					
					default:
						darg = (signed long long int) va_arg(ap, signed int);
						break;
					}
					
					if (darg == 0 && f.p.isset && f.p.width == 0)
						break;
					
					/* forced sign */
					if (f.f.sign)
						buflen += fmt_plusminus(&ibuf[buflen], darg);
					else
						buflen += fmt_minus(&ibuf[buflen], darg);
					
					darg = darg < 0 ? -darg : darg;
					
					len = fmt_ulonglong(FMT_LEN, darg);
					
					while (len < f.p.width) {
						buflen += fmt_str(&ibuf[buflen], "0");
						f.p.width--;
					}
					
					buflen += fmt_ulonglong(&ibuf[buflen], darg);
					break;
				
				/* unsigned conversions */
				case 'o': /* octal notation */
				case 'u': /* decimal notation */
				case 'x': /* hexadecimal notation */
					if (c == 'o') {
						alt = "0";
						ufn = fmt_8longlong;
					}
					
					else if (c == 'u') {
						alt = "";
						ufn = fmt_ulonglong;
					}
					
					else {
						alt = "0x";
						ufn = fmt_xlonglong;
					}
					
					switch (f.l) {
					case 0:
						uarg = (unsigned char) va_arg(ap, unsigned int);
						break;
					
					case 1:
						uarg = (unsigned short int) va_arg(ap, unsigned int);
						break;
					
					case 2:
						uarg = (unsigned int) va_arg(ap, unsigned int);
						break;
					
					case 3:
						uarg = (unsigned long int) va_arg(ap, unsigned long int);
						break;
					
					case 4:
						uarg = (unsigned long long int) va_arg(ap, unsigned long long int);
						break;
					
					default:
						uarg = (unsigned long long int) va_arg(ap, unsigned int);
						break;
					}
					
					if (uarg == 0 && f.p.isset && f.p.width == 0)
						break;
					
					if (f.f.alt)
						buflen += fmt_str(&ibuf[buflen], alt);
						
					len = ufn(FMT_LEN, uarg);
					
					while (len < f.p.width) {
						buflen += fmt_str(&ibuf[buflen], "0");
						f.p.width--;
					}
					
					buflen += ufn(&ibuf[buflen], uarg);
					break;
				
				case 'f': /* float conversion */
					farg = va_arg(ap, double);
					
					if (!f.p.isset)
						f.p.width = 6;
					
					/* forced sign (fmt_double adds minus) */
					if (f.f.sign && farg >= 0)
						buflen += fmt_str(&ibuf[buflen], "+");
					
					buflen += fmt_double(&ibuf[buflen], farg, f.p.width);
					break;
				
				case 'c': /* character conversion */
					/* no zero padding for string conversions */
					f.f.zero = 0;
					
					carg = va_arg(ap, int);
					ibuf[buflen++] = (unsigned char) carg;
					break;
				
				case 's': /* string conversion */
					/* no zero padding for string conversions */
					f.f.zero = 0;
					
					sarg = va_arg(ap, const char *);
					buf = sarg;
					
					if (!buf) {
						buf    = "(null)";
						buflen = 6;
					}
					
					else if (!f.p.isset)
						buflen = str_len(buf);
					
					else if (f.p.width == 0)
						buflen = 0;
					
					else {
						const char *q = str_index(buf, '\0', f.p.width);
						
						if (q == 0) /* string will be truncated */
							buflen = f.p.width;
						else
							buflen = q - buf;
					}
					
					break;
					
				case 'p': /* pointer conversion */
					parg = va_arg(ap, void *);
					
					if (f.f.alt)
						buflen += fmt_str(&ibuf[buflen], "0x");
					
					buflen += fmt_xlonglong(&ibuf[buflen], (unsigned long int)parg);
					break;
				
				case 'n':
					narg  = va_arg(ap, int *);
					*narg = idx;
					
					break;
				
				case '%':
					/* no padding for % */
					f.w = 0;
					
					ibuf[buflen++] = '%';
				
				default:
					/* no padding for unknown conversion */
					f.w = 0;
					
					buf    = fmtp;
					buflen = str_len(fmtp);
					fmt    = fmtp + buflen;
					
					const char *q = str_index(buf + 1, '%', buflen - 1);
					
					if (q != 0) {
						buflen = q - buf - 1;
						fmt    = q - 1;
					}
					
					break;
			}
			
			/* blank/zero padding using right alignment */
			if (!f.f.left) {
				while (f.w > buflen) {
					if (idx < size) {
						if (f.f.zero)
							fmt_strn(&str[idx++], "0", 1);
						else
							fmt_strn(&str[idx++], " ", 1);
					}
					
					else
						idx++;
					
					f.w--;
				}
			}
			
			/* write the converted argument */
			for (i = 0; i < buflen; i++) {
				if (idx < size)
					fmt_strn(&str[idx++], &buf[i], 1);
				else
					idx++;
			}
			
			/* blank/zero padding using left alignment */
			if (f.f.left && f.w > i) {
				while (f.w > buflen) {
					if (idx < size) {
						if (f.f.zero)
							fmt_strn(&str[idx++], "0", 1);
						else
							fmt_strn(&str[idx++], " ", 1);
					}
					
					else
						idx++;
					
					f.w--;
				}
			}
			
			fmtp = &c;
		}
	}
	
	va_end(ap);
	
	return idx;
}
