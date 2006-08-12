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
 * @defgroup mmap Map files into memory
 * @{
 */

#ifndef _LUCID_MMAP_H
#define _LUCID_MMAP_H

#include <sys/types.h>

/*!
 * @brief map file into memory read-only
 *
 * @param filename path to file to be mapped
 * @param len      bytes to be mapped
 *
 * @return pointer to mapped memory
 */
char *mmap_read(const char *filename, size_t *len);

/*!
 * @brief map file into a private memory copy
 *
 * @param filename path to file to be mapped
 * @param len      bytes to be mapped
 *
 * @return pointer to mapped memory
 */
char *mmap_private(const char *filename, size_t *len);

/*!
 * @brief map file into memory read-write
 *
 * @param filename path to file to be mapped
 * @param len      bytes to be mapped
 *
 * @return pointer to mapped memory
 */
char *mmap_shared(const char *filename, size_t *len);

#endif

/*! @} mmap */