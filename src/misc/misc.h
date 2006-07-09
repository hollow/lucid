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
 * @defgroup misc Miscellaneous
 * @{
 */

#ifndef _LUCID_MISC_H
#define _LUCID_MISC_H

#include <sys/types.h>

/*!
 * @brief check if given path is a directory
 *
 * @param path path to check
 *
 * @return 1 on success, 0 otherwise
 */
int isdir  (const char *path);

/*!
 * @brief check if given path is a regular file
 *
 * @param path path to check
 *
 * @return 1 on success, 0 otherwise
 */
int isfile (const char *path);

/*!
 * @brief check if given path is a symbolic link
 *
 * @param path path to check
 *
 * @return 1 on success, 0 otherwise
 */
int islink (const char *path);

/*!
 * @brief recursive mkdir(2) with dirname(3)
 *
 * @param path path to create
 * @param mode file permissions
 *
 * @return 0 on success, -1 on error with errno set
 */
int mkdirnamep(const char *path, mode_t mode);

/*!
 * @brief recursive mkdir(2)
 *
 * @param path path to create
 * @param mode file permissions
 *
 * @return 0 on success, -1 on error with errno set
 */
int mkdirp (const char *path, mode_t mode);

/*!
 * @brief concatenate dirname and basename
 *
 * @param dirname  directory part
 * @param basename basename part
 *
 * @return path on success, NULL on error with errno set
 */
char *path_concat(char *dirname, char *basename);

/*!
 * @brief recursive unlink(2) and rmdir(2)
 *
 * @param path path to remove
 *
 * @return 0 on success, -1 on error with errno set
 */
int runlink(const char *path);

#endif

/*! @} misc */
