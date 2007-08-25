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
 * @defgroup printf Formatted output conversion
 *
 * The  functions in the printf() family produce output according to a format as
 * described below.
 *
 * The functions printf() and vprintf() write output to stdout, the standard
 * output stream; dprintf() and vdprintf() write output to the file descriptor
 * fd; asprintf() and vasprintf() allocate a string long enough to hold the
 * output; snprintf() and vsnprintf() write to the character string str.
 *
 * The functions vprintf(), vdprintf(), vasprintf() and vsnprintf() are
 * equivalent to the functions printf(), dprintf(), asprintf() and snprintf(),
 * respectively, except that they are called with a va_list instead  of a
 * variable number of arguments. These functions do not call the va_end macro.
 * Consequently, the value of ap is undefined after the call. The application
 * should call va_end(ap) itself afterwards.
 *
 * @section format Format of the format string
 *
 * The format string is composed of zero or more directives: ordinary characters
 * (not %), which are copied unchanged to the output stream; and conversion
 * specifications, each of which results in fetching zero or more subsequent
 * arguments.  Each conversion specification is introduced by the character %,
 * and ends with a conversion specifier. In between there may be (in this order)
 * zero or more flags, an optional minimum field width, an optional precision
 * and an optional length modifier.
 *
 * @subsection flags The flag characters
 *
 * The character % is followed by zero or more of the following flags:
 *
 * - #<br>
 *   The value should be converted to an ``alternate form''. For o conversions,
 *   the first character of the output string is made zero (by prefixing a 0 if
 *   it was not zero already). For x conversions, the result has the
 *   string `0x' prepended to it. For other conversions, the result is
 *   undefined.
 * - 0<br>
 *   The value should be zero padded. For d, i, o, u, x, and f conversions, the
 *   converted value is padded on the left with zeros rather than blanks. If the
 *   0 and - flags both appear, the 0 flag is ignored. If a precision is given
 *   with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
 *   For other conversions, the behavior is undefined.
 * - -<br>
 *   The converted value is to be left adjusted on the field boundary. (The
 *   default is right justification.) Except for n conversions, the converted
 *   value is padded on the right with blanks, rather than on the left with
 *   blanks or zeros. A - overrides a 0 if both are given.
 * - ' '<br>
 *   (a space) A blank should be left before a positive number (or empty string)
 *   produced by a signed conversion.
 * - +<br>
 *   A sign (+ or -) should always be placed before a number produced by a
 *   signed conversion. By default a sign is used only for negative numbers.
 *   A + overrides a space if both are used.
 *
 * @subsection width The field width
 *
 * An optional decimal digit string (with non-zero first digit) specifying a
 * minimum field width. If the converted value has fewer characters than the
 * field width, it will be padded with spaces on the left (or right, if the
 * left-adjustment flag has been given). Instead of a decimal digit string one
 * may write `*' to specify that the field width is given in the next argument,
 * which must be of type int. A negative field width is taken as a `-' flag
 * followed by a positive field width. In no case does a non-existent or small
 * field width cause truncation of a field; if the result of a conversion is
 * wider than the field width, the field is expanded to contain the conversion
 * result.
 *
 * @subsection length The length modifier
 *
 * Here, `integer conversion' stands for d, i, o, u, or x conversion.
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
 * Note however that internally, hh, h, and l are handled as long int, ll as
 * long long int, respectively.
 *
 * @subsection conversion The conversion specifier
 *
 * A character that specifies the type of conversion to be applied. The
 * conversion specifiers and their meanings are:
 *
 * - d,i<br>
 *   The int argument is converted to signed decimal notation. The precision, if
 *   any, gives the minimum number of digits that must appear; if the converted
 *   value requires fewer digits, it is padded on the left with zeros. The
 *   default precision is 1. When 0 is printed with an explicit precision 0,
 *   the output is empty.
 * - o,u,x,X<br>
 *   The unsigned int argument is converted to unsigned octal (o), unsigned
 *   decimal (u), or unsigned hexadecimal (x) notation. The letters abcdef are
 *   used for x conversions. The precision, if any, gives the minimum number of
 *   digits that must appear; if the converted value requires fewer digits, it
 *   is padded on the left with zeros. The default precision is 1. When 0 is
 *   printed with an explicit precision 0, the output is empty.
 * - c<br>
 *   The int argument is converted to an unsigned char, and the resulting
 *   character is written.
 * - s<br>
 *   The const char * argument is expected to be a pointer to an array of
 *   character type (pointer to a string). Characters from the array are written
 *   up to (but not including) a terminating null byte ('\\0'); if a precision is
 *   specified, no more than the number specified are written. If a precision is
 *   given, no null byte need be present; if the precision is not specified, or
 *   is greater than the size of the array, the array must contain a terminating
 *   null byte.
 * - p<br>
 *   The void * pointer argument is printed in hexadecimal.
 * - n<br>
 *   The number of characters written so far is stored into the integer
 *   indicated by the int * pointer argument. No argument is converted.
 * - %<br>
 *   A `%' is written. No argument is converted. The complete conversion
 *   specification is `%%'.
 *
 * @section conform Note on conformance
 *
 * This printf implementation is not fully C99 or SUS compliant, though most
 * common features are implemented in a completely self-contained way, to make
 * integration within other applications as easy as possible.
 *
 * @see fmt
 *
 * @{
 */

#ifndef _LUCID_PRINTF_H
#define _LUCID_PRINTF_H

#include <stdarg.h>

/*!
 * @brief write conversion to string using va_list
 *
 * @param[out] str  buffer to store conversion
 * @param[in]  size size of str
 * @param[in]  fmt  format string
 * @param[in]  ap   variable number of arguments
 *
 * @return number of bytes (that would have been) written
 *
 * @note Every conversion happens in this functions. All other printf functions
 *       are just convenient wrappers.
 */
int _lucid_vsnprintf(char *str, int size, const char *fmt, va_list ap);

/*!
 * @brief write conversion to string using variable number of arguments
 *
 * @param[out] str  buffer to store conversion
 * @param[in]  size size of str
 * @param[in]  fmt  format string
 * @param[in]  ...  variable number of arguments
 *
 * @return number of bytes (that would have been) written
 */
int _lucid_snprintf (char *str, int size, const char *fmt, /*args*/ ...);

/*!
 * @brief write conversion to allocated string using va_list
 *
 * @param[out] ptr  pointer to string to store conversion
 * @param[in]  fmt  format string
 * @param[in]  ap   variable number of arguments
 *
 * @return number of bytes (that would have been) written
 *
 * @see malloc(3)
 * @see free(3)
 */
int _lucid_vasprintf(char **ptr, const char *fmt, va_list ap);

/*!
 * @brief write conversion to allocated string using variable number of arguments
 *
 * @param[out] ptr  pointer to string to store conversion
 * @param[in]  fmt  format string
 * @param[in]  ...  variable number of arguments
 *
 * @return number of bytes (that would have been) written
 *
 * @see malloc(3)
 * @see free(3)
 */
int _lucid_asprintf(char **ptr, const char *fmt, /*args*/ ...);

/*!
 * @brief write conversion to file descriptor using va_list
 *
 * @param[in] fd   open file descriptor
 * @param[in] fmt  format string
 * @param[in] ap   variable number of arguments
 *
 * @return number of bytes (that would have been) written
 */
int _lucid_vdprintf(int fd, const char *fmt, va_list ap);

/*!
 * @brief write conversion to file descriptor using variable number of arguments
 *
 * @param[in] fd   open file descriptor
 * @param[in] fmt  format string
 * @param[in] ...  variable number of arguments
 *
 * @return number of bytes (that would have been) written
 */
int _lucid_dprintf(int fd, const char *fmt, /*args*/ ...);

/*!
 * @brief write conversion to stdout using va_list
 *
 * @param[in] fmt  format string
 * @param[in] ap   variable number of arguments
 *
 * @return number of bytes (that would have been) written
 */
int _lucid_vprintf(const char *fmt, va_list ap);

/*!
 * @brief write conversion to stdout using variable number of arguments
 *
 * @param[in] fmt  format string
 * @param[in] ...  variable number of arguments
 *
 * @return number of bytes (that would have been) written
 */
int _lucid_printf(const char *fmt, /*args*/ ...);

#ifdef _LUCID_PRINTF_MACROS
#define vsnprintf _lucid_vsnprintf
#define snprintf  _lucid_snprintf
#define vasprintf _lucid_vasprintf
#define asprintf  _lucid_asprintf
#define vdprintf  _lucid_vdprintf
#define dprintf   _lucid_dprintf
#define vprintf   _lucid_vprintf
#define printf    _lucid_printf
#endif

#endif

/*! @} printf */
