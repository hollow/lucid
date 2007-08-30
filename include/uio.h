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

#ifndef _LUCID_UIO_H
#define _LUCID_UIO_H

/*!
 * @defgroup uio Universal Input/Output
 *
 * @{
 */

int uio_open(const char *filename, const char *flags, mode_t mode);

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
int uio_read(int fd, char **str, int len);

/*!
 * @brief read a line of input
 *
 * @param[in]  fd   file descriptor to read from
 * @param[out] str  pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for line using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int uio_read_eol(int fd, char **line);

/*!
 * @brief read until end of file
 *
 * @param[in]  fd   file descriptor to read from
 * @param[out] str  pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for file using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int uio_read_eof(int fd, char **str);

/*!
 * @brief close a file
 *
 * @param[in] fd file descriptor
 */
void uio_close(int fd);

/*!
 * @brief check if given path is a directory
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int uio_isdir(const char *path);

/*!
 * @brief check if given path is a regular file
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int uio_isfile(const char *path);

/*!
 * @brief check if given path is a symbolic link
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int uio_islink(const char *path);

/*!
 * @brief check if given path exists
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int uio_ispath(const char *path);

/*!
 * @brief check if given path is a top-level mount point
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int uio_ismount(const char *path);

/*!
 * @brief recursive mkdir(2)
 *
 * @param[in] path path to create
 * @param[in] mode file permissions
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see mkdir(2)
 */
int uio_mkdir(const char *path, mode_t mode);

/*!
 * @brief recursive mkdir(2) with dirname(3)
 *
 * @param[in] path path to create
 * @param[in] mode file permissions
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see mkdir(2)
 * @see dirname(3)
 */
int uio_mkdirname(const char *path, mode_t mode);

/*!
 * @brief read contents of symlink
 *
 * @param[in] path symlink to read
 *
 * @return on success a pointer to a string containing the destination of the
 *         link, NULL on error with errno set
 *
 * @see unlink(2)
 * @see rmdir(2)
 */
char *uio_readlink(const char *path);

/*!
 * @brief recursive unlink(2) and rmdir(2)
 *
 * @param[in] path path to remove
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see unlink(2)
 * @see rmdir(2)
 */
int uio_unlink(const char *path);

/*!
 * @brief copy a file
 *
 * @param[in] srcfd filedescriptor to read from
 * @param[in] dstfd filedescriptor to write to
 *
 * @return 0 on success, -1 on error with errno set
 */
int uio_copy(int srcfd, int dstfd);

#endif

/*! @} uio */
