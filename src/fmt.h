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

/*!
 * @defgroup fmt Formatted integer conversion
 *
 * The fmt family of functions provide the core of the printf family of
 * functions; i.e. the conversion of single elements. Each function only
 * converts a single value at a time.
 *
 * The fmt_8longlong() function converts an unsigned long long int to octal
 * notation and stores the conversion in the string pointed to by dst.
 *
 * The fmt_longlong() function converts an signed long long int to decimal
 * notation and stores the conversion in the string pointed to by dst.
 *
 * The fmt_ulonglong() function converts an unsigned long long int to decimal
 * notation and stores the conversion in the string pointed to by dst.
 *
 * The fmt_xlonglong() function converts an unsigned long long int to
 * hexadecimal notation and stores the conversion in the string pointed to by
 * dst.
 *
 * The fmt_double() function converts a double to decimal notation in the
 * style [-]ddd.ddd, where the number of digits after the decimal-point
 * character is equal to the precision specification.
 *
 * The fmt_minus() function converts a negative long long int to the minus
 * character and stores the conversion in the string pointed to by dst. The
 * fmt_plusminus() additionally converts a positive long long int to the plus
 * character.
 *
 * The fmt_str() and fmt_strn() functions append the string pointed to by src to
 * the string pointed to by dst.
 *
 * @{
 */

#ifndef _LUCID_FMT_H
#define _LUCID_FMT_H

/*! @brief enough space to hold -2^127 in decimal, plus \\0 */
#define FMT_LONG  41

/*! @brief enough space to hold 2^128 - 1 in decimal, plus \\0 */
#define FMT_ULONG 40

/*! @brief enough space to hold 2^128 - 1 in octal, plus \\0 */
#define FMT_8LONG 44

/*! @brief enough space to hold 2^128 - 1 in hexadecimal, plus \\0 */
#define FMT_XLONG 33

/*! @brief convenient abbreviation to count string length */
#define FMT_LEN ((char *) 0)

/*!
 * @brief write integer to string in octal notation
 *
 * @param[out] dst dstination string
 * @param[in]  src integer to write
 *
 * @return number of bytes written to dst
 */
int fmt_8longlong(char *dst, unsigned long long int src);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dst destination string
 * @param[in]  src integer to write
 *
 * @return number of bytes written to dst
 */
int fmt_longlong(char *dst, signed long long int src);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dst destination string
 * @param[in]  src integer to write
 *
 * @return number of bytes written to dst
 */
int fmt_ulonglong(char *dst, unsigned long long int src);

/*!
 * @brief write integer to string in hexadecimal notation
 *
 * @param[out] dst destination string
 * @param[in]  src integer to write
 *
 * @return number of bytes written to dst
 */
int fmt_xlonglong(char *dst, unsigned long long int src);

/*!
 * @brief write float to string in decimal notation
 *
 * @param[out] dst  destination string
 * @param[in]  src  double-precision float to write
 * @param[in]  prec significand precision
 *
 * @return number of bytes written to dst
 */
int fmt_double(char *dst, double src, int prec);

/*!
 * @brief write sign to string
 *
 * @param[out] dst destination string
 * @param[in]  src integer to write
 *
 * @return number of bytes written to dst
 */
int fmt_plusminus(char *dst, signed long long int src);

/*!
 * @brief write minus sign to string
 *
 * @param[out] dst destination string
 * @param[in]  src integer to write
 *
 * @return number of bytes written to dst
 */
int fmt_minus(char *dst, signed long long int src);

/*!
 * @brief write string to string
 *
 * @param[out] dst destination string
 * @param[in]  src source string
 *
 * @return Number of bytes written to dst
 */
int fmt_str(char *dst, const char *src);

/*!
 * @brief write string to string
 *
 * @param[out] dst destination string
 * @param[in]  src source string
 * @param[in]  n   maximum number of bytes to copy
 *
 * @return Number of bytes written to dst
 */
int fmt_strn(char *dst, const char *src, int n);

#endif

/*! @} fmt */
