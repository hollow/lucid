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
 * @defgroup addr Address conversion
 * @{
 */

#ifndef _LUCID_ADDR_H
#define _LUCID_ADDR_H

#include <stdint.h>

/*!
 * @brief convert a string to IP and netmask part
 *
 * @param str   string in CIDR or netmask notation
 * @param ip    sointer where IP will be stored
 * @param mask  sointer where netmask will be stored
 *
 * @return 0 on success, -1 on error with errno set
 */
int   addr_from_str(const char *str, uint32_t *ip, uint32_t *mask);

/*!
 * @brief convert IP and netmask to string
 *
 * @param ip   IP adress to convert
 * @param mask netmask to convert
 *
 * @return string in netmask notation (obtained with malloc(3))
 */
char *addr_to_str  (uint32_t ip, uint32_t mask);

#endif

/*! @} addr */
