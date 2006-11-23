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
 * @defgroup addr Internet address conversion
 *
 * addr_from_str() converts the Internet host address str from the standard
 * numbers-and-dots notation into binary data and stores it in the ip/mask pair
 * of pointers. addr_from_str() returns 0 if no argument was converted, 1 if ip
 * was converted, 2 for mask and 3 for both.
 *
 * The addr_to_str() function converts the Internet host address given in the
 * ip/mask pair of pointers to a string in standard numbers-and-dots notation.
 * The string is returned is obtained by malloc and should be free(3)'d by the
 * caller.
 *
 * @{
 */

#ifndef _LUCID_ADDR_H
#define _LUCID_ADDR_H

#include <stdint.h>

/*!
 * @brief convert string to IP address and netmask
 *
 * @param[in]  str   string in CIDR or netmask notation
 * @param[out] ip    pointer to store IP address 
 * @param[out] mask  pointer to store netmask
 *
 * @return 0 if no argument was converted, 1 if ip was
 *            converted, 2 for mask and 3 for both.
 */
int addr_from_str(const char *str, uint32_t *ip, uint32_t *mask);

/*!
 * @brief convert IP address and netmask to string
 *
 * @param[in] ip   IP adress to convert
 * @param[in] mask netmask to convert
 *
 * @return string in netmask notation (obtained with malloc(3))
 *
 * @note The caller should free obtained memory using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 */
char *addr_to_str(uint32_t ip, uint32_t mask);

#endif

/*! @} addr */
