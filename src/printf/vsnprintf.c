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

#include <string.h>

#include "fmt/fmt.h"
#include "printf/printf.h"
#include "str/str.h"

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
	unsigned int width;
} __prec_t;

typedef struct {
	__flags_t f;
	__prec_t  p;
	unsigned int w;
	unsigned int l;
	unsigned int c;
} __printf_t;

/* supported formats:
** - format flags: #, 0, -, ' ', +
** - field width
** - argument precision
** - length mods: hh, h, l, z (handled as long); ll (handled as long long)
** - conversion spec: d, i, u, o, x, f, c, s, p */
int _lucid_vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
	/* keep track of string length */
	size_t idx = 0;
	
	while (*fmt) {
		if (*fmt != '%') {
			if (idx < size)
				str[idx++] = *fmt++;
			else {
				idx++;
				fmt++;
			}
		}
		
		else {
			/* skip % */
			fmt++;
			
			/* format struct */
			__printf_t f;
			
			/* sanitize struct with default values */
			f.f.alt   = 0;
			f.f.zero  = 0;
			f.f.left  = 0;
			f.f.blank = 0;
			f.f.sign  = 0;
			
			f.w = 0;
			
			f.p.isset = 0;
			f.p.width = 1;
			
			f.l = '\0';
			f.c = '\0';
			
			/* parse flags */
			while (*fmt == '0' || *fmt == '-' || *fmt == '+' || *fmt == ' ' || *fmt == '#') {
				switch (*fmt) {
					case '#': f.f.alt   = 1; break;
					case '0': f.f.zero  = 1; break;
					case '-': f.f.left  = 1; break;
					case ' ': f.f.blank = 1; break;
					case '+': f.f.sign  = 1; break;
				}
				
				fmt++;
			}
			
			/* parse field width */
			if (*fmt == '*') {
				fmt++;
				
				int arg = va_arg(ap, int);
				
				if (arg >= 0)
					f.w = arg;
				else {
					f.w = -arg;
					f.f.left = 1;
				}
			} else if (char_isdigit(*fmt)) {
				f.w = *fmt - '0';
				
				fmt ++;
				
				while (char_isdigit(*fmt)) {
					f.w = 10 * f.w + (unsigned int)(*fmt - '0');
					fmt++;
				}
			}
			
			/* parse precision */
			if (*fmt == '.') {
				fmt++;
				
				f.p.isset = 1;
				f.p.width = 0;
				
				if (*fmt == '*') {
					fmt++;
					
					int arg = va_arg(ap, int);
					
					if (arg >= 0)
						f.p.width = arg;
				} else if (char_isdigit(*fmt)) {
					f.p.width = *fmt - '0';
					
					fmt++;
					
					while (char_isdigit(*fmt)) {
						f.p.width = 10 * f.p.width + (unsigned int)(*fmt - '0');
						fmt++;
					}
				}
			}
			
			/* parse length modifier
			** hh, h and l, z are all handled as long int
			** ll, L is handled as long long int */
			switch (*fmt) {
				case 'h':
				case 'z':
					f.l = 'l';
					
					if (*(fmt+1) == 'h')
						fmt++;
					
					break;
				
				case 'l':
					f.l = 'l';
					
					if (*(fmt+1) == 'l') {
						f.l = '2';
						fmt++;
					}
					
					break;
				
				case 'L':
					f.l = '2';
					break;
				
				default: break;
			}
			
			if (f.l != '\0')
				fmt++;
			
			/* parse conversion specifier */
			switch (*fmt) {
				case 'd':
				case 'i':
					f.c = 'd';
					break;
				
				case 'f':
				
				case 'o':
				case 'u':
				case 'x':
				
				case 'c':
				case 's':
				
				case 'p':
				
				case '%':
					f.c = *fmt;
					break;
				
				default: break;
			}
			
			fmt++;
			
			/* sign overrides blank */
			if (f.f.sign)
				f.f.blank = 0;
			
			/* left overrides zero */
			if (f.f.left)
				f.f.zero = 0;
			
			/* no zero padding if precision is specified */
			if (f.p.isset && f.w > 0)
				f.f.zero = 0;
			
			/* no zero padding for string conversions */
			if (f.c == 'c' || f.c == 's')
				f.f.zero = 0;
			
			/* signed long argument */
			signed long darg; signed long long ldarg;
			
			/* unsigned long argument */
			unsigned long uarg; unsigned long long luarg;
			
			/* float argument */
			double farg;
			
			/* character argument */
			int carg;
			
			/* string argument */
			char *sarg;
			
			/* pointer argument */
			void *parg;
			
			/* used for precision counting */
			int len;
			
			/* temporary buffer for integer conversions */
			char ibuf[64];
			
			/* temporary buffer for string conversion */
			char *buf = ibuf;
			
			/* buffer length */
			size_t buflen = 0;
			
			/* do conversions */
			switch (f.c) {
				case 'd': /* signed conversion */
					if (f.l == '2') { /* long long */
						ldarg = va_arg(ap, signed long long);
						
						if (ldarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						/* forced sign */
						if (f.f.sign) {
							if (f.f.blank || ldarg == 0)
								buflen += fmt_str(&ibuf[buflen], " ");
							else
								buflen += fmt_plusminus(&ibuf[buflen], ldarg);
						} else {
							buflen += fmt_minus(&ibuf[buflen], ldarg);
						}
						
						len = fmt_ulonglong(FMT_LEN, (unsigned long long) ldarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
					
						buflen += fmt_ulonglong(&ibuf[buflen], (unsigned long long) ldarg);
					} else { /* !long long */
						darg = va_arg(ap, signed long);
						
						if (darg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						/* forced sign */
						if (f.f.sign) {
							if (f.f.blank || darg == 0)
								buflen += fmt_str(&ibuf[buflen], " ");
							else
								buflen += fmt_plusminus(&ibuf[buflen], darg);
						} else {
							buflen += fmt_minus(&ibuf[buflen], darg);
						}
						
						len = fmt_ulong(FMT_LEN, (unsigned long) darg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
					
						buflen += fmt_ulong(&ibuf[buflen], (unsigned long) darg);
					}
					break;
				
				case 'f': /* float conversion */
					farg = va_arg(ap, double);
					
					if (!f.p.isset)
						f.p.width = 6;
					
					/* forced sign */
					if (f.f.sign) {
						if (f.f.blank || farg == 0)
							buflen += fmt_str(&ibuf[buflen], " ");
						else
							buflen += fmt_plusminus(&ibuf[buflen], farg);
					} else {
						buflen += fmt_minus(&ibuf[buflen], farg);
					}
					
					len = fmt_double(FMT_LEN, farg, f.p.width);
					
					while ((unsigned long) len < f.p.width) {
						buflen += fmt_str(&ibuf[buflen], "0");
						f.p.width--;
					}
					
					buflen += fmt_double(&ibuf[buflen], farg, f.p.width);
					break;
				
				/* unsigned conversions */
				case 'o': /* octal representation */
					if (f.l == '2') { /* long long */
						uarg = va_arg(ap, unsigned long);
						
						if (uarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						if (f.f.alt)
							buflen += fmt_str(&ibuf[buflen], "0");
						
						len = fmt_8long(FMT_LEN, uarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
						
						buflen += fmt_8long(&ibuf[buflen], uarg);
					} else { /* !long long */
						luarg = va_arg(ap, unsigned long long);
						
						if (luarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						if (f.f.alt)
							buflen += fmt_str(&ibuf[buflen], "0");
						
						len = fmt_8longlong(FMT_LEN, luarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
						
						buflen += fmt_8longlong(&ibuf[buflen], luarg);
					}
					
					break;
				
				case 'u': /* decimal representation */
					if (f.l == '2') {
						luarg = va_arg(ap, unsigned long long);
						
						if (luarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						len = fmt_ulonglong(FMT_LEN, luarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
					
						buflen += fmt_ulonglong(&ibuf[buflen], luarg);
					} else {
						uarg = va_arg(ap, unsigned long);
						
						if (uarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						len = fmt_ulong(FMT_LEN, uarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
					
						buflen += fmt_ulong(&ibuf[buflen], uarg);
					}
					break;
				
				case 'x': /* hex conversion */
					if (f.l == '2') {
						luarg = va_arg(ap, unsigned long long);
						
						if (luarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						if (f.f.alt)
							buflen += fmt_str(&ibuf[buflen], "0x");
						
						len = fmt_xlonglong(FMT_LEN, luarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
						
						buflen += fmt_xlonglong(&ibuf[buflen], luarg);
					} else {
						uarg = va_arg(ap, unsigned long);
						
						if (uarg == 0 && f.p.isset && f.p.width == 0)
							break;
						
						if (f.f.alt)
							buflen += fmt_str(&ibuf[buflen], "0x");
						
						len = fmt_xlong(FMT_LEN, uarg);
						
						while ((unsigned long) len < f.p.width) {
							buflen += fmt_str(&ibuf[buflen], "0");
							f.p.width--;
						}
						
						buflen += fmt_xlong(&ibuf[buflen], uarg);
					}
					break;
				
				case 'c': /* character conversion */
					carg = va_arg(ap, int);
					ibuf[buflen++] = (char) carg;
					break;
				
				case 's': /* string conversion */
					sarg = va_arg(ap, char *);
					buf = sarg;
					
					if (!buf) buflen = 0;
					else if (!f.p.isset) buflen = strlen(buf);
					else if (f.p.width == 0) buflen = 0;
					else {
						const char *q = memchr(buf, '\0', f.p.width);
						
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
					
					buflen += fmt_xlong(&ibuf[buflen], (unsigned long) parg);
					break;
				
				default: break;
			}
			
			/* blank/zero padding using right alignment */
			if (!f.f.left) {
				while (f.w > buflen) {
					if (idx < size) {
						if (f.f.zero)
							fmt_strn(&str[idx++], "0", 1);
						else
							fmt_strn(&str[idx++], " ", 1);
					} else
						idx++;
					
					f.w--;
				}
			}
			
			size_t i;
			
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
					} else
						idx++;
					
					f.w--;
				}
			}
		}
	}
	
	if (idx < size)
		str[idx] = '\0';
	
	return idx;
}
