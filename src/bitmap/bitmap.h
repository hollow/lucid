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
 * @defgroup bitmap Bitmap conversion
 * @{
 */

#ifndef _LUCID_BITMAP_H
#define _LUCID_BITMAP_H

#include <stdint.h>

/*!
 * @brief convert bit index to 32 bit value
 *
 * @param index bit index (0-31)
 *
 * @return 32 bit value
 */
uint32_t i2v32(int index);

/*!
 * @brief convert bit index to 64 bit value
 *
 * @param index bit index (0-63)
 *
 * @return 64 bit value
 */
uint64_t i2v64(int index);

/*!
 * @brief convert 32 bit value to bit index
 *
 * @param val 32 bit value
 *
 * @return bit index (0-31)
 */
int v2i32(uint32_t val);

/*!
 * @brief convert 64 bit value to bit index
 *
 * @param val 64 bit value
 *
 * @return bit index (0-63)
 */
int v2i64(uint64_t val);

#endif

/*! @} bitmap */
