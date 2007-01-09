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
#include "scanf.h"
#include "str.h"

enum __scanf_flags {
	SFL_NOOP  = 0x01,
	SFL_WIDTH = 0x02,
};

enum __scanf_rank {
	SFR_CHAR,
	SFR_SHORT,
	SFR_INT,
	SFR_LONG,
	SFR_LLONG,
};

#define SFR_MIN SFR_CHAR
#define SFR_MAX SFR_LLONG

enum __scanf_state {
	SFS_NORMAL,
	SFS_FLAGS,
	SFS_WIDTH,
	SFS_MOD,
	SFS_CONV,
	SFS_STORE,
	SFS_EOF,
	SFS_ERR,
};

typedef struct {
	int f; /* flags */
	int l; /* length */
	int s; /* state */
	int w; /* width */
} __scanf_t;

/* supported formats:
** - field width
** - length mods: hh, h, l, and ll
** - conversion spec: d, i, u, o, x, X, c, s, p, P, n */
int _lucid_vsscanf(const char *str, const char *fmt, va_list _ap)
{
	/* keep track of converted arguments */
	int converted = 0;
	
	/* current character in format */
	char c;
	
	/* current conversion data */
	__scanf_t f;
	
	/* arguments */
	union {
		/* unsigned argument */
		unsigned long long int u;
		
		/* string argument */
		char *s;
	} arg;
	
	/* base used for integer conversions */
	int base;
	
	/* number of bytes converted in str_toumax */
	int len;
	
	/* pointer for string conversion */
	char *sp;
	
	/* don't consume original ap */
	va_list ap;
	va_copy(ap, _ap);
	
	/* initialize conversion data */
	f.f = 0;
	f.l = SFR_INT;
	f.s = SFS_NORMAL;
	f.w = str_len(str);
	
	while ((c = *fmt++)) {
		switch (f.s) {
		case SFS_NORMAL:
			if (c == '%') {
				f.f = 0;
				f.l = SFR_INT;
				f.s = SFS_FLAGS;
				f.w = str_len(str);
			}
			
			else if (char_isspace(c))
				while (char_isspace(*str))
					str++;
			
			else if (*str == c)
				str++;
			
			else
				f.s = SFS_ERR;
			
			break;
		
		case SFS_FLAGS:
			switch (c) {
			case '*':
				f.f |= SFL_NOOP;
				break;
			
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				f.w  = (c - '0');
				f.f |= SFL_WIDTH;
				f.s  = SFS_WIDTH;
				break;
			
			default:
				f.s = SFS_MOD;
				fmt--;
				break;
			}
			
			break;
		
		case SFS_WIDTH:
			if (c >= '0' && c <= '9')
				f.w = f.w * 10  + (c - '0');
			
			else {
				f.s = SFS_MOD;
				fmt--;
			}
			
			break;
		
		case SFS_MOD:
			switch (c) {
			case 'h':
				f.l--;
				break;
			
			case 'l':
				f.l++;
				break;
			
			default:
				f.s = SFS_CONV;
				fmt--;
				break;
			}
			
			break;
		
		case SFS_CONV:
			f.s = SFS_NORMAL;
			
			if (f.l > SFR_MAX)
				f.l = SFR_MAX;
			
			if (f.l < SFR_MIN)
				f.l = SFR_MIN;
			
			switch (c) {
			case 'd':
				base = 10;
				goto scan_int;
			
			case 'i': /* signed conversion */
				base = 0;
				goto scan_int;
			
			case 'o':
				base = 8;
				goto scan_int;
			
			case 'u':
				base = 10;
				goto scan_int;
			
			case 'X':
			case 'x':
				base = 16;
				goto scan_int;
			
			scan_int:
				while (char_isspace(*str))
					str++;
				
				if (!*str) {
					f.s = SFS_EOF;
					break;
				}
				
				len = str_toumax(str, &arg.u, base, f.w);
				
				if (len <= 0) {
					f.s = SFS_ERR;
					break;
				}
				
				str += len;
				converted++;
				
				if (!(f.f & SFL_NOOP)) {
					switch (f.l) {
					case SFR_CHAR:
						*va_arg(ap, unsigned char *) = arg.u;
						break;
					
					case SFR_SHORT:
						*va_arg(ap, unsigned short int *) = arg.u;
						break;
					
					case SFR_INT:
						*va_arg(ap, unsigned int *) = arg.u;
						break;
					
					case SFR_LONG:
						*va_arg(ap, unsigned long int *) = arg.u;
						break;
					
					case SFR_LLONG:
						*va_arg(ap, unsigned long long int *) = arg.u;
						break;
					
					default:
						*va_arg(ap, unsigned long long int *) = arg.u;
						break;
					}
				}
				
				break;
			
			case 'c': /* character conversion */
				/* default width = 1 */
				f.w = (f.f & SFL_WIDTH) ? f.w : 1;
				
				if ((f.f & SFL_NOOP)) {
					while (f.w--) {
						if (!*str) {
							f.s = SFS_EOF;
							break;
						}
						
						str++;
					}
				}
				
				else {
					arg.s = va_arg(ap, char *);
					
					while (f.w--) {
						if (!*str) {
							f.s = SFS_EOF;
							break;
						}
						
						*arg.s++ = *str++;
					}
				}
				
				if (f.s != SFS_EOF && !(f.f & SFL_NOOP))
					converted++;
				
				break;
			
			case 's': /* string conversion */
				if (!(f.f & SFL_NOOP)) {
					while (f.w-- && !char_isspace(*str)) {
						if (!*str) {
							f.s = SFS_EOF;
							break;
						}
						
						str++;
					}
				}
				
				else {
					sp = arg.s = va_arg(ap, char *);
					
					while (f.w-- && !char_isspace(*str)) {
						if (!*str) {
							f.s = SFS_EOF;
							break;
						}
						
						*sp++ = *str++;
					}
					
					if (f.s != SFS_EOF)
						*sp = '\0';
				}
				
				if (f.s != SFS_EOF && !(f.f & SFL_NOOP))
					converted++;
				
				break;
			
			case 'P':
			case 'p': /* pointer conversion */
				while (char_isspace(*str))
					str++;
				
				if (!*str) {
					f.s = SFS_EOF;
					break;
				}
				
				len = str_toumax(str, &arg.u, 0, f.w);
				
				if (len <= 0) {
					f.s = SFS_ERR;
					break;
				}
				
				if (!(f.f & SFL_NOOP))
					*va_arg(ap, void **) = (void *) (unsigned long int) arg.u;
				
				str += len;
				converted++;
				
				break;
			
			case 'n':
				*va_arg(ap, int *) = converted;
				
				break;
			
			case '%':
				if (*str == '%')
					str++;
				else
					f.s = SFS_ERR;
				
				break;
			
			default:
				f.s = SFS_ERR;
				break;
			}
			
			break;
			
		case SFS_EOF:
			converted = converted ? converted : -1;
			
		case SFS_ERR:
			va_end(ap);
			return converted;
		}
	}
	
	if (f.s == SFS_EOF)
		converted = converted ? converted : -1;
	
	va_end(ap);
	return converted;
}
