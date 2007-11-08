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

/*!
 * @defgroup scanf Formatted input conversion
 *
 * The scanf() family of functions scans input according to format as described
 * below. This format may contain conversion specifications; the results from
 * such conversions, if any, are stored in the locations pointed to by the
 * pointer arguments that follow format. Each pointer argument must be of a type
 * that is appropriate for the value returned by the corresponding conversion
 * specification.
 *
 * If the number of conversion specifications in format exceeds the number of
 * pointer arguments, the results are undefined. If the number of pointer
 * arguments exceeds the number of conversion specifications, then the excess
 * pointer arguments are evaluated, but are otherwise ignored.
 *
 * The format string consists of a sequence of directives which describe how to
 * process the sequence of input characters. If processing of a directive fails,
 * no further input is read, and scanf() returns. A "failure" can be either of
 * the following: input failure, meaning that input characters were unavailable,
 * or matching failure, meaning that the input was inappropriate (see below).
 *
 * A directive is one of the following:
 *
 * - A sequence of white-space characters (space, tab, newline, etc; see
 *   isspace(3)). This directive matches any amount of white space, including
 *   none, in the input.
 *
 * - An ordinary character (i.e., one other than white space or '%'). This
 *   character must exactly match the next character of input.
 *
 * - A conversion specification, which commences with a '%' (percent) character.
 *   A sequence of characters from the input is converted according to this
 *   specification, and the result is placed in the corresponding pointer
 *   argument. If the next item of input does not match the the conversion
 *   specification, the conversion fails — this is a matching failure.
 *
 * @section format Format of the format string
 *
 * Each conversion specification  in format  begins with either the character
 * '%' followed by:
 *
 * - An optional '*' assignment-suppression character: scanf() reads input as
 *   directed by the conversion specification, but discards the input. No
 *   corresponding pointer argument is required, and this specification is not
 *   included in the count of successful assignments returned by scanf().
 *
 * - An optional decimal integer which specifies the maximum field width.
 *   Reading of characters stops either when this maximum is reached or when a
 *   non-matching character is found, whichever happens first. Most conversions
 *   discard initial whitespace characters (the exceptions are noted below),
 *   and these discarded characters don't count towards the maximum field
 *   width. String input conversions store a null terminator ('\\0') to mark the
 *   end of the input; the maximum field width does not include this
 *  terminator.
 *
 * - An optional type modifier character. For example, the l type modifier is
 *   used with integer conversions such as %d  to specify that the
 *   corresponding pointer argument refers to a long int rather than a pointer
 *   to an int.
 *
 * - A conversion specifier that specifies the type of input conversion to be
 *   performed.
 *
 * @subsection conversions Conversions
 *
 * The following type modifier characters can appear in a conversion
 * specification:
 *
 * - hh<br>
 *   A following integer conversion corresponds to a signed char or unsigned
 *   char argument, or a following n conversion corresponds to a pointer to a
 *   signed char argument.
 * - h<br>
 *   A following integer conversion corresponds to a short int or unsigned short
 *   int argument, or a following n conversion corresponds to a pointer to a
 *   short int argument.
 * - l<br>
 *   (ell) A following integer conversion corresponds to a long int or unsigned
 *    long int argument.
 * - ll<br>
 *   (ell-ell). A following integer conversion corresponds to a long long int or
 *   unsigned long long int argument.
 *
 * The following conversion specifiers are available:
 *
 * - %<br>
 *   Matches a literal '%'. That is, %%  in the format string matches a single
 *   input '%' character. No conversion is done, and assignment does not occur.
 * - d<br>
 *   Matches an optionally signed decimal integer; the next pointer must be a
 *   pointer to int.
 * - i<br>
 *   Matches an optionally signed integer; the next pointer must be a pointer
 *   to int. The integer is read in base 16 if it begins with 0x or 0X, in base
 *   8 if it begins with 0, and in base 10 otherwise. Only characters that
 *   correspond to the base are used.
 * - o<br>
 *   Matches an unsigned octal integer; the next pointer must be a pointer to
 *   unsigned int.
 * - u<br>
 *   Matches an unsigned decimal integer; the next pointer must be a pointer to
 *   unsigned int.
 * - x,X<br>
 *   Matches an unsigned hexadecimal integer; the next pointer must be a
 *   pointer to unsigned int.
 * - s<br>
 *   Matches a sequence of non-white-space characters; the next pointer must be
 *   a pointer to character array that is long enough to hold the input
 *   sequence and the terminating null character ('\\0'), which is added
 *   automatically. The input string stops at white space or at the maximum
 *   field width, whichever occurs first.
 * - c<br>
 *   Matches a sequence of characters whose length is specified by the maximum
 *   field width (default 1); the next pointer must be a pointer to char, and
 *   there must be enough room for all the characters (no terminating null byte
 *   is added). The usual skip of leading white space is suppressed. To skip
 *   white space first, use an explicit space in the format.
 * - p<br>
 *   Matches a pointer value (as printed by %p in printf(3); the next pointer
 *   must be a pointer to a pointer to void.
 * - n<br>
 *   Nothing is expected; instead, the number of characters consumed thus far
 *   from the input is stored through the next pointer, which must be a pointer
 *   to int. This is not a conversion, although it can be suppressed with the *
 *   assignment-suppression character.
 *
 * @{
 */

#ifndef _LUCID_SCANF_H
#define _LUCID_SCANF_H

#include <stdarg.h>

/*!
 * @brief read conversion from string using va_list
 *
 * @param[in]  str  source string
 * @param[in]  fmt  format string
 * @param[out] ap   variable number of arguments
 *
 * @return Number of converted arguments
 *
 * @note Every conversion happens in this functions. All other scanf functions
 *       are just convenient wrappers.
 */
int _lucid_vsscanf(const char *str, const char *fmt, va_list ap);

/*!
 * @brief read conversion from string using variable number of arguments
 *
 * @param[in]  str  source string
 * @param[in]  fmt  format string
 * @param[out] ...  variable number of arguments
 *
 * @return Number of converted arguments
 */
int _lucid_sscanf(const char *str, const char *fmt, /*args*/ ...);

#define vsscanf _lucid_vsscanf
#define sscanf  _lucid_sscanf

#endif

/*! @} printf */
