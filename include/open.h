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
 * @defgroup open Create or open files
 *
 * The open family of functions provide wrappers around open(2) with different
 * flags.
 *
 * @{
 */

#ifndef _LUCID_OPEN_H
#define _LUCID_OPEN_H

/*!
 * @brief open file in append mode
 *
 * @param filename file to open
 *
 * @return filedescriptor on success, -1 otherwise with errno set
 *
 * @see open(2)
 */
int open_append(const char *filename);

/*!
 * @brief open file exclusively
 *
 * @param filename file to open
 *
 * @return filedescriptor on success, -1 otherwise with errno set
 *
 * @see open(2)
 */
int open_excl(const char *filename);

/*!
 * @brief open file for reading
 *
 * @param filename file to open
 *
 * @return filedescriptor on success, -1 otherwise with errno set
 *
 * @see open(2)
 */
int open_read(const char *filename);

/*!
 * @brief open file for reading and writing
 *
 * @param filename file to open
 *
 * @return filedescriptor on success, -1 otherwise with errno set
 *
 * @see open(2)
 */
int open_rw(const char *filename);

/*!
 * @brief open and truncate file for reading and writing
 *
 * @param filename file to open
 *
 * @return filedescriptor on success, -1 otherwise with errno set
 *
 * @see open(2)
 */
int open_trunc(const char *filename);

/*!
 * @brief open file for writing
 *
 * @param filename file to open
 *
 * @return filedescriptor on success, -1 otherwise with errno set
 *
 * @see open(2)
 */
int open_write(const char *filename);

#endif

/*! @} open */
