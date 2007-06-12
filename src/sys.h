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
 * @defgroup sys System calls
 *
 * @{
 */

#ifndef _LUCID_SYS_H
#define _LUCID_SYS_H

#include <sys/types.h>

/*!
 * @brief get memory page size
 *
 * @return Returns the number of bytes in a page, where a "page" is the thing
 * used where it says in the description of mmap(2) that files are mapped in
 * page-sized units.
 */
int sys_getpagesize(void);

/*!
 * @brief map files or devices into memory
 *
 * @return On success, mmap() returns a pointer to the mapped area. On error,
 * the value MAP_FAILED (that is, (void *) -1) is returned, and errno is set
 * appropriately.
 */
void *sys_mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset);

/*!
 * @brief re-map a virtual memory address
 *
 * @return On success mremap() returns a pointer to the new virtual memory
 * area. On error, the value MAP_FAILED (that is, (void *) -1) is returned, and
 * errno is set appropriately.
 */
void *sys_mremap(void *old_address, size_t old_size, size_t new_size, int flags);

/*!
 * @brief unmap files or devices from memory
 *
 * @return On success, munmap() returns 0, on failure -1, and errno is set
 * (probably to EINVAL).
 */
int sys_munmap(void *start, size_t length);

#endif

/*! @} sys */
