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
 * @defgroup misc Miscellaneous helpers
 *
 * The misc family of functions provide wrappers not fitting in any other
 * module and not being worth an own category for each of them.
 *
 * The isdir(), isfile() and islink() functions wrap the stat(2) system call and
 * checks if the path in the string pointed to by path is a directory, regular
 * file or link, respectively.
 *
 * The mkdirp() function creates any missing parent directories of the path in
 * the string pointed to by path, before creating the directory itself. The
 * mkdirnamep() function additionally calls dirname(3) on the path string
 * before calling mkdirp().
 *
 * The path_concat() function concatenates the strings pointed to by dirname and
 * basename and checks the latter using str_path_isdot().
 *
 * The runlink() function removes all files and directories in the path pointed
 * to by the string path.
 *
 * @{
 */

#ifndef _LUCID_MISC_H
#define _LUCID_MISC_H

#include <sys/types.h>

/*!
 * @brief check if given path exists
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int ispath(const char *path);

/*!
 * @brief check if given path is a directory
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int isdir(const char *path);

/*!
 * @brief check if given path is a regular file
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int isfile(const char *path);

/*!
 * @brief check if given path is a symbolic link
 *
 * @param[in] path path to check
 *
 * @return 1 on success, 0 otherwise
 *
 * @see stat(2)
 */
int islink(const char *path);

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
int mkdirnamep(const char *path, mode_t mode);

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
int mkdirp(const char *path, mode_t mode);

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
int runlink(const char *path);

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
char *readsymlink(const char *path);

/*!
 * @brief copy a file
 *
 * @param[in] src path to read from
 * @param[in] dst path to write to
 *
 * @return 0 on success, -1 on error with errno set
 */
int copy_file(const char *src, const char *dst);

#endif

/*! @} misc */
