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
 * @defgroup cext Extensions for the C standard library
 *
 * @{
 */

#ifndef _LUCID_CEXT_H
#define _LUCID_CEXT_H

/*!
 * @brief duplicate a memory block
 *
 * @param[in] s pointer to source memory area
 * @param[in] n duplicate first n bytes
 *
 * @return A pointer to the duplicated memory block, or NULL if insufficient
 *         memory was available.
 */
void *memdup(const void *s, int n);

#endif

/*! @} mem */
