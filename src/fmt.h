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

#ifndef _LUCID_FMT_H
#define _LUCID_FMT_H

#include <sys/types.h>

/* enough space to hold -2^127 in decimal, plus \0 */
#define FMT_LONG  41

/* enough space to hold 2^128 - 1 in decimal, plus \0 */
#define FMT_ULONG 40

/* enough space to hold 2^128 - 1 in octal, plus \0 */
#define FMT_8LONG 44

/* enough space to hold 2^128 - 1 in hexadecimal, plus \0 */
#define FMT_XLONG 33

/* convenient abbreviation */
#define FMT_LEN ((char *) 0)

/* octal conversion */
size_t fmt_8long    (char *dest, unsigned long i);
size_t fmt_8longlong(char *dest, unsigned long long i);

/* signed decimal conversion */
size_t fmt_long    (char *dest, long i);
size_t fmt_longlong(char *dest, long long i);

/* unsigned decimal conversion */
size_t fmt_ulong    (char *dest, unsigned long i);
size_t fmt_ulonglong(char *dest, unsigned long long i);

/* hex conversion */
size_t fmt_xlong    (char *dest, unsigned long i);
size_t fmt_xlonglong(char *dest, unsigned long long i);

/* floating point conversion */
size_t fmt_double(char *dest, double d, size_t prec);

/* print plus/minus characters */
size_t fmt_plusminus(char *dest, long long i);
size_t fmt_minus    (char *dest, long long i);

/* string concatenation */
size_t fmt_str (char *dest, const char *src);
size_t fmt_strn(char *dest, const char *src, size_t n);

/* string alignment */
size_t fmt_pad (char *dest, const char *src, size_t srclen, size_t padlen);
size_t fmt_fill(char *dest,                  size_t srclen, size_t padlen);

#endif
