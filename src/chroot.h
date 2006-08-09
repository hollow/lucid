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
 * @defgroup chroot Secure chroot
 * @{
 */

#ifndef _LUCID_CHROOT_H
#define _LUCID_CHROOT_H

#include <sys/types.h>

/*!
 * @brief chroot(2) to the directory pointed to by a filedescriptor
 *
 * @param fd file descriptor refering to a directory (fchdir(2))
 *
 * @return 0 on success, -1 on error with errno set
 */
int chroot_fd(int fd);

/*!
 * @brief recursive mkdir(2) inside a secure chroot
 *
 * @param root new root path
 * @param dir  dir to be created in root
 * @param mode file permissions
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @see chroot_secure_chdir
 */
int chroot_mkdirp(const char *root, const char *dir, mode_t mode);

/*!
 * @brief symlink-attack safe chroot(2)
 *
 * @param root new root path
 * @param dir  dir to to chdir(2) in root
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @note this function uses a well-known chroot(2) exploit to get a secure
 *       filedescriptor inside the new root but still reamining in the old
 *       root in the caller
 */
int chroot_secure_chdir(const char *root, const char *dir);

#endif

/*! @} chroot */
