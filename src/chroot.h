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
 * @defgroup chroot Secure chroot wrappers
 *
 * The chroot system call changes the root directory of the current process.
 * This directory will be used for pathnames beginning with /. The root
 * directory is inherited by all children of the current process.
 *
 * The chroot family of functions provide wrappers for other library functions
 * to happen in a chroot while the caller still remains in the old root after
 * the functions have returned.
 *
 * One can break out of the chroot in many ways due to the nature of the chroot
 * system call:
 *
 * - This call changes an ingredient in the pathname resolution process and does
 *   nothing else.
 * - This call does not change the current working directory.
 * - This call does not close open file descriptors.
 *
 * The main usage of these functions is to get a file descriptor, safe against
 * symlink attacks, reffering to a directory inside a new root.
 * @{
 */

#ifndef _LUCID_CHROOT_H
#define _LUCID_CHROOT_H

#include <sys/types.h>

/*!
 * @brief chroot(2) to the directory pointed to by a filedescriptor
 *
 * @param[in] fd file descriptor refering to a directory (fchdir(2))
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see chroot(2)
 * @see fchdir(2)
 */
int chroot_fd(int fd);

/*!
 * @brief recursive mkdir(2) inside a secure chroot
 *
 * @param[in] root new root path
 * @param[in] dir  dir to be created in root
 * @param[in] mode file permissions
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see chroot_secure_chdir
 * @see mkdir(2)
 */
int chroot_mkdirp(const char *root, const char *dir, mode_t mode);

/*!
 * @brief symlink-attack safe chdir(2) in chroot(2)
 *
 * @param[in] root new root path
 * @param[in] dir  dir to chdir(2) in root
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see chroot(2)
 * @see chdir(2)
 */
int chroot_secure_chdir(const char *root, const char *dir);

#endif

/*! @} chroot */
