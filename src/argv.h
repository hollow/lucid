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
 * @{
 */

#ifndef _LUCID_ARGV_H
#define _LUCID_ARGV_H

/*!
 * @brief convert string to argument vector
 *
 * @param str  space seperated string to convert
 * @param argv argument vector to fill
 * @param argc size of argv
 *
 * @return number of arguments found/converted
 *
 * @note the caller is responsible to alloc memory for argv
 * @note this function modifies its first argument, use with caution
 */
int argv_from_str(char *str, char ** const argv, int argc);

/*!
 * @brief convert an argument vector to a string
 *
 * @param argc size of argv
 * @param argv argument vector to convert
 *
 * @return space seperated string (obtained by malloc(3))
 */
char *argv_to_str(int argc, const char ** const argv);

#endif

/*! @} argv */
