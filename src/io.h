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
 * @defgroup io Input/Output wrappers
 *
 * The io family of functions provide convenient wrappers around read(2).
 *
 * The io_read_eol() function reads bytes until the end of the line, that is 
 * until one of the characters \\r or \\n is found, and stores them in the
 * string pointed to by line.
 *
 * The io_read_eof() function reads bytes until the end of the file is reached
 * and stores them in the string pointed to by file.
 *
 * The io_read_len() function reads an exact number of bytes from the file and
 * stores them in the string pointed to by str.
 *
 * @{
 */

#ifndef _LUCID_IO_H
#define _LUCID_IO_H

/*! bytes read at a time */
#define CHUNKSIZE 128

/*!
 * @brief read a line of input
 *
 * @param[in]  fd   file descriptor to read from
 * @param[out] line pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for line using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int io_read_eol(int fd, char **line);

/*!
 * @brief read until end of file
 *
 * @param[in]  fd   file descriptor to read from
 * @param[out] file pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for file using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int io_read_eof(int fd, char **file);

/*!
 * @brief read exact number of bytes
 *
 * @param[in]  fd  file descriptor to read from
 * @param[out] str pointer to a string
 * @param[in]  len bytes to be read
 *
 * @return bytes read on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for str using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int io_read_len(int fd, char **str, size_t len);

#endif

/*! @} io */
