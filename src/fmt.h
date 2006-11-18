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
 * @defgroup fmt Formatted string conversion
 *
 * The fmt family of functions provide the core of the printf family of
 * functions; i.e. the conversion of single elements. Each function only
 * converts a single value at a time.
 *
 * The fmt_8long() and fmt_8longlong() functions convert a unsigned long int or
 * unsigned long long int, respectively, to octal notation and stores the
 * conversion in the string pointed to by dest.
 *
 * The fmt_long() and fmt_longlong() functions convert a long int or
 * long long int, respectively, to decimal notation and stores the
 * conversion in the string pointed to by dest.
 *
 * The fmt_ulong() and fmt_ulonglong() functions convert a unsigned long int or
 * unsigned long long int, respectively, to decimal notation and stores the
 * conversion in the string pointed to by dest.
 *
 * The fmt_xlong() and fmt_xlonglong() functions convert a unsigned long int or
 * unsigned long long int, respectively, to hexadecimal notation and stores the
 * conversion in the string pointed to by dest.
 *
 * The fmt_double() function converts a double to decimal notation in the
 * style [-]ddd.ddd, where the number of digits after the decimal-point
 * character is equal to the precision specification.
 *
 * The fmt_minus() function converts a negative long long int to the minus
 * character and stores the conversion in the string pointed to by dest. The
 * fmt_plusminus() additionally converts a positive long long int to the plus
 * character.
 *
 * The fmt_str() and fmt_strn() functions append the string pointed to by src to
 * the string pointed to by dest.
 *
 * The fmt_pad() and fmt_fill() functions prefix or postfix the string pointed
 * to by dest with blanks, respectively.
 *
 * @{
 */

#ifndef _LUCID_FMT_H
#define _LUCID_FMT_H

#include <sys/types.h>

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
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_8long(char *dest, unsigned long i);

/*!
 * @brief write integer to string in octal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_8longlong(char *dest, unsigned long long i);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_long(char *dest, long i);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_longlong(char *dest, long long i);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_ulong(char *dest, unsigned long i);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_ulonglong(char *dest, unsigned long long i);

/*!
 * @brief write integer to string in hexadecimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_xlong(char *dest, unsigned long i);

/*!
 * @brief write integer to string in hexadecimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_xlonglong(char *dest, unsigned long long i);

/*!
 * @brief write float to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  d    double-precision float to write
 * @param[in]  prec significand precision
 *
 * @return number of bytes written to dest
 */
size_t fmt_double(char *dest, double d, size_t prec);

/*!
 * @brief write sign to string
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_plusminus(char *dest, long long i);

/*!
 * @brief write minus sign to string
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
size_t fmt_minus(char *dest, long long i);

/*!
 * @brief write string to string
 *
 * @param[out] dest destination string
 * @param[in]  src  source string
 *
 * @return number of bytes written to dest
 */
size_t fmt_str(char *dest, const char *src);

/*!
 * @brief write string to string
 *
 * @param[out] dest destination string
 * @param[in]  src  source string
 * @param[in]  n    maximum number of bytes to copy
 *
 * @return number of bytes written to dest
 */
size_t fmt_strn(char *dest, const char *src, size_t n);

/*!
 * @brief pad string with blanks
 *
 * @param[out] dest   destination string
 * @param[in]  src    source string
 * @param[in]  srclen length of source string
 * @param[in]  padlen (padded) length of destination string
 *
 * @return number of bytes written to dest
 */
size_t fmt_pad(char *dest, const char *src, size_t srclen, size_t padlen);

/*!
 * @brief fill string with blanks
 *
 * @param[out] dest   destination string
 * @param[in]  srclen length of source string
 * @param[in]  padlen (padded) length of destination string
 *
 * @return number of bytes written to dest
 */
size_t fmt_fill(char *dest, size_t srclen, size_t padlen);

#endif

/*! @} fmt */
