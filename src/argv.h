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

/*!
 * @defgroup argv Argument vector conversion
 *
 * The command line arguments are the whitespace-separated tokens given in the
 * shell command used to invoke the program; thus, in `cat foo bar', the
 * arguments are `foo' and `bar'. For the command `cat foo bar', argc is 3 and
 * argv has three elements, "cat", "foo" and "bar".
 *
 * The argv_from_str() and argv_to_str() functions convert an argument vector
 * (as in main()) from and to a string (as seen in the shell), respectively.
 *
 * @{
 */

#ifndef _LUCID_ARGV_H
#define _LUCID_ARGV_H

/*!
 * @brief convert string to argument vector
 *
 * @param[in]  str  space seperated string to convert
 * @param[out] argv argument vector to fill
 * @param[in]  argc size of argv
 *
 * @return number of arguments found/converted
 *
 * @note the caller has to allocate memory for argv
 * @note this function modifies its first argument, use with caution
 */
int argv_from_str(char *str, char ** const argv, int argc);

/*!
 * @brief convert argument vector to string
 *
 * @param[in] argc size of argv
 * @param[in] argv argument vector to convert
 *
 * @return space seperated string (obtained by malloc(3))
 *
 * @note The caller should free obtained memory using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 */
char *argv_to_str(int argc, const char ** const argv);

#endif

/*! @} argv */
