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
 * @defgroup bitmap Bitmap conversion
 *
 * The i2v and v2i family of functions convert between a bitmap and a bit index.
 *
 * A bitmap is simply an integer with certain bits being 1 (enabled) and 0
 * (disabled).
 *
 * These functions only return usable results if exactly one bit is enabled.
 *
 * - Bit index to bitmap<br>
 *   The resulting bitmask is a simple arithmetic left shift of 1 index times.
 * - Bitmap to bit index<br>
 *   The resulting bit index is asimple arithmetic right shift until the map is
 *   empty.
 *
 * These functions are mainly used by the flist family of functions.
 *
 * @{
 */

#ifndef _LUCID_BITMAP_H
#define _LUCID_BITMAP_H

#include <stdint.h>

/*!
 * @brief convert bit index to 32 bit value
 *
 * @param[in] index bit index (0-31)
 *
 * @return 32 bit value
 */
uint32_t i2v32(int index);

/*!
 * @brief convert bit index to 64 bit value
 *
 * @param[in] index bit index (0-63)
 *
 * @return 64 bit value
 */
uint64_t i2v64(int index);

/*!
 * @brief convert 32 bit value to bit index
 *
 * @param[in] val 32 bit value
 *
 * @return bit index (0-31)
 */
int v2i32(uint32_t val);

/*!
 * @brief convert 64 bit value to bit index
 *
 * @param[in] val 64 bit value
 *
 * @return bit index (0-63)
 */
int v2i64(uint64_t val);

#endif

/*! @} bitmap */
