// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#include "printf.h"
#include "str.h"

enum __printf_flags {
	PFL_ALT    = 0x01,
	PFL_ZERO   = 0x02,
	PFL_LEFT   = 0x04,
	PFL_BLANK  = 0x08,
	PFL_SIGN   = 0x10,
	PFL_UPPER  = 0x20,
	PFL_SIGNED = 0x40,
};

enum __printf_rank {
	PFR_CHAR,
	PFR_SHORT,
	PFR_INT,
	PFR_LONG,
	PFR_LLONG,
};

#define PFR_MIN PFR_CHAR
#define PFR_MAX PFR_LLONG

enum __printf_state {
	PFS_NORMAL,
	PFS_FLAGS,
	PFS_WIDTH,
	PFS_PREC,
	PFS_MOD,
	PFS_CONV,
};

typedef struct {
	unsigned int f; /* flags */
	int l;          /* length */
	int p;          /* precision */
	int s;          /* state */
	unsigned int w; /* width */
} __printf_t;

#define EMIT(C) { if (idx < size - 1) { *str++ = C; } idx++; }

static
int __printf_int(char *str, int size,
                 unsigned long long int val, int base,
                 __printf_t f)
{
	static const char lcdigits[] = "0123456789abcdef";
	static const char ucdigits[] = "0123456789ABCDEF";
	const char *digits;
	
	int idx = 0, ndigits = 0, nchars, minus = 0;
	unsigned long long int tmpval;
	
	/* select type of digits */
	digits = (f.f & PFL_UPPER) ? ucdigits : lcdigits;
	
	/* separate out the minus */
	if (f.f & PFL_SIGNED && (signed long long int) val < 0) {
		minus = 1;
		val = (unsigned long long int) (-(signed long long int) val);
	}
	
	/* count number of digits needed */
	tmpval = val;
	
	while (tmpval) {
		tmpval /= base;
		ndigits++;
	}
	
	/* zero still requires space */
	if (val == 0)
		ndigits = 1;
	
	/* compute number of nondigits */
	nchars = f.p > ndigits ? f.p : ndigits;
	
	/* need space for sign */
	if (minus || (f.f & (PFL_SIGN | PFL_BLANK)))
		nchars++;
	
	/* add prefix for alternate form */
	if (f.f & PFL_ALT) {
		if (base == 16)
			nchars += 2;
		else if (base == 8 && val != 0)
			nchars += 1;
	}
	
	/* early space padding */
	if (!(f.f & (PFL_LEFT | PFL_ZERO)) && f.w > nchars) {
		while (f.w > nchars) {
			EMIT(' ')
			f.w--;
		}
	}
	
	/* zero padding */
	if ((f.f & (PFL_LEFT | PFL_ZERO)) == PFL_ZERO && f.w > nchars) {
		while (f.w > nchars) {
			EMIT('0')
			f.w--;
		}
	}
	
	/* nondigits */
	if (minus)
		EMIT('-')
	
	else if (f.f & PFL_SIGN)
		EMIT('+')
	
	else if (f.f & PFL_BLANK)
		EMIT(' ')
	
	if (f.f & PFL_ALT) {
		if (base == 16) {
			EMIT('0')
			EMIT((f.f & PFL_UPPER) ? 'X' : 'x');
		}
		
		else if (base == 8 && val != 0)
			EMIT('0')
	}
	
	/* precision */
	if (f.p > ndigits) {
		while (f.p > ndigits) {
			EMIT('0')
			f.p--;
		}
	}
	
	/* generate the number from right to left */
	str += ndigits;
	
	do {
		if (idx < size)
			*--str = digits[val % base];
		
		idx++;
	} while ((val /= base));
	
	/* late space padding */
	while ((f.f & PFL_LEFT) && f.w > nchars) {
		EMIT(' ')
		f.w--;
	}
	
	return idx;
}

/* supported formats:
** - format flags: #, 0, -, ' ', +
** - field width
** - argument precision
** - length mods: hh, h, l, and ll
** - conversion spec: d, i, u, o, x, X, c, s, p, P, n */
int _lucid_vsnprintf(char *str, int size, const char *fmt, va_list _ap)
{
	/* generic counter */
	int i;
	
	/* generic pointer */
	const char *p;
	
	/* keep track of string length */
	int idx = 0;
	
	/* save pointer to start of current conversion */
	const char *ccp = fmt;
	
	/* current character in format */
	char c;
	
	/* current conversion data */
	__printf_t f;
	
	/* arguments */
	union {
		/* signed argument */
		signed long long int d;
		
		/* unsigned argument */
		unsigned long long int u;
		
		/* float argument */
		double f;
		
		/* character argument */
		int c;
		
		/* string argument */
		const char *s;
		
		/* pointer argument */
		void *p;
		
		/* number argument */
		int *n;
	} arg;
	
	/* base used for integer conversions */
	int base;
	
	/* number of consumed bytes in conversions */
	int len;
	
	/* don't consume original ap */
	va_list ap;
	va_copy(ap, _ap);
	
	/* initialize conversion data */
	f.f = 0;
	f.l = PFR_INT;
	f.p = -1;
	f.s = PFS_NORMAL;
	f.w = 0;
	
	if (size > 0)
		mem_set(str, 0, size);
	
	while ((c = *fmt++)) {
		switch (f.s) {
		case PFS_NORMAL:
			if (c == '%') {
				f.f = 0;
				f.l = PFR_INT;
				f.p = -1;
				f.s = PFS_FLAGS;
				f.w = 0;
				ccp = &c;
			}
			
			else
				EMIT(c)
			
			break;
		
		case PFS_FLAGS:
			switch (c) {
			case '#':
				f.f |= PFL_ALT;
				break;
			
			case '0':
				f.f |= PFL_ZERO;
				break;
			
			case '-':
				f.f &= ~PFL_ZERO; /* left overrides zero */
				f.f |=  PFL_LEFT;
				break;
			
			case ' ':
				f.f |= PFL_BLANK;
				break;
			
			case '+':
				f.f &= ~PFL_BLANK; /* sign overrides blank */
				f.f |=  PFL_SIGN;
				break;
			
			default:
				f.s = PFS_WIDTH;
				fmt--;
				break;
			}
			
			break;
		
		case PFS_WIDTH:
			if (c == '-') {
					f.f &= PFL_ZERO; /* left overrides zero */
					f.f |= PFL_LEFT;
			}
			
			else if (c >= '0' && c <= '9')
				f.w = f.w * 10  + (c - '0');
			
			else if (c == '*') {
				f.w = va_arg(ap, int);
				
				if (f.w < 0) {
					f.w = -f.w;
					f.f &= PFL_ZERO; /* left overrides zero */
					f.f |= PFL_LEFT;
				}
			}
			
			else if (c == '.') {
				f.p = 0;
				f.s = PFS_PREC;
			}
			
			else {
				f.s = PFS_MOD;
				fmt--;
			}
			
			break;
		
		case PFS_PREC:
			if (c >= '0' && c <= '9')
				f.p = f.p * 10  + (c - '0');
			
			else if (c == '*') {
				f.p = va_arg(ap, int);
				
				if (f.p < 0)
					f.p = 0;
			}
			
			else {
				f.s = PFS_MOD;
				fmt--;
			}
			
			break;
		
		case PFS_MOD:
			switch (c) {
			case 'h':
				f.l--;
				break;
			
			case 'l':
				f.l++;
				break;
			
			default:
				f.s = PFS_CONV;
				fmt--;
				break;
			}
			
			break;
		
		case PFS_CONV:
			f.s = PFS_NORMAL;
			
			if (f.l > PFR_MAX)
				f.l = PFR_MAX;
			
			if (f.l < PFR_MIN)
				f.l = PFR_MIN;
			
			switch (c) {
			case 'P':
				f.f |= PFL_UPPER;
			
			case 'p':
				base = 16;
				f.p  = (8 * sizeof(void *) + 3)/4;
				f.f |= PFL_ALT;
				
				arg.u = (unsigned long long int) (unsigned long int) va_arg(ap, void *);
				
				goto is_integer;
			
			case 'd':
			case 'i': /* signed conversion */
				base = 10;
				f.f |= PFL_SIGNED;
				
				switch (f.l) {
				case PFR_CHAR:
					arg.d = (signed char) va_arg(ap, signed int);
					break;
				
				case PFR_SHORT:
					arg.d = (signed short int) va_arg(ap, signed int);
					break;
				
				case PFR_INT:
					arg.d = (signed int) va_arg(ap, signed int);
					break;
				
				case PFR_LONG:
					arg.d = (signed long int) va_arg(ap, signed long int);
					break;
				
				case PFR_LLONG:
					arg.d = (signed long long int) va_arg(ap, signed long long int);
					break;
				
				default:
					arg.d = (signed long long int) va_arg(ap, signed int);
					break;
				}
				
				arg.u = (unsigned long long int) arg.d;
				
				goto is_integer;
			
			case 'o':
				base = 8;
				goto is_unsigned;
				
			case 'u':
				base = 10;
				goto is_unsigned;
				
			case 'X':
				f.f |= PFL_UPPER;
			
			case 'x':
				base = 16;
				goto is_unsigned;
				
			is_unsigned:
				switch (f.l) {
				case PFR_CHAR:
					arg.u = (unsigned char) va_arg(ap, unsigned int);
					break;
				
				case PFR_SHORT:
					arg.u = (unsigned short int) va_arg(ap, unsigned int);
					break;
				
				case PFR_INT:
					arg.u = (unsigned int) va_arg(ap, unsigned int);
					break;
				
				case PFR_LONG:
					arg.u = (unsigned long int) va_arg(ap, unsigned long int);
					break;
				
				case PFR_LLONG:
					arg.u = (unsigned long long int) va_arg(ap, unsigned long long int);
					break;
				
				default:
					arg.u = (unsigned long long int) va_arg(ap, unsigned int);
					break;
				}
				
			is_integer:
				len = __printf_int(str, size, arg.u, base, f);
				
				str += len;
				idx += len;
				break;
			
			case 'c': /* character conversion */
				arg.c = (char) va_arg(ap, int);
				EMIT(arg.c)
				break;
			
			case 's': /* string conversion */
				arg.s = va_arg(ap, const char *);
				arg.s = arg.s ? arg.s : "(null)";
				len   = str_len(arg.s);
				
			is_string:
				if (f.p != -1 && len > f.p)
					len = f.p;
				
				while (f.w-- > len && !(f.f & PFL_LEFT)) {
					if (f.f & PFL_ZERO)
						EMIT('0')
					else
						EMIT(' ')
				}
				
				for (i = len; i; i--)
					EMIT(*arg.s++)
				
				while (f.w-- > len && (f.f & PFL_LEFT))
					EMIT(' ')
				
				break;
				
			case 'n':
				arg.n  = va_arg(ap, int *);
				*arg.n = idx;
				
				break;
			
			case '%':
				EMIT(c)
				break;
			
			default:
				/* no padding for unknown conversion */
				f.w =  0;
				f.p = -1;
				
				arg.s = ccp;
				len   = str_len(arg.s);
				fmt   = ccp + len;
				
				p = str_chr(arg.s + 1, '%', len - 1);
				
				if (p != 0) {
					len = p - arg.s - 1;
					fmt = p - 1;
				}
				
				goto is_string;
			}
			
			break;
		}
	}
	
	va_end(ap);
	
	return idx;
}
