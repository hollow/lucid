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
 * @defgroup io Input/Output
 * @{
 */

#ifndef _LUCID_IO_H
#define _LUCID_IO_H

/*! bytes read at a time */
#define CHUNKSIZE 128

/*!
 * @brief read a line of input
 *
 * @param fd   file descriptor to read from
 * @param line pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note the caller is responsible to free memory for line obtained by malloc(3)
 */
int io_read_eol(int fd, char **line);

/*!
 * @brief read until end of file
 *
 * @param fd   file descriptor to read from
 * @param file pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note the caller is responsible to free memory for file obtained by malloc(3)
 */
int io_read_eof(int fd, char **file);

/*!
 * @brief read exact number of bytes
 *
 * @param fd  file descriptor to read from
 * @param str pointer to a string
 * @param len bytes to be read
 *
 * @return bytes read on success, -1 on error with errno set
 *
 * @note the caller is responsible to free memory for str obtained by malloc(3)
 */
int io_read_len(int fd, char **str, size_t len);

#endif

/*! @} io */
