// Copyright 2005 Felix von Leitner <felix-libowfat@fefe.de>
//           2006 Benedikt Böhm <hollow@gentoo.org>
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

#ifndef _LUCID_SCN_H
#define _LUCID_SCN_H

/*!
 * @brief write integer to string in octal notation
 *
 * @param[out] dst destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
int scn_8longlong(const char *src, unsigned long long int *dst);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
int scn_longlong(const char *src, signed long long int *dst);

/*!
 * @brief write integer to string in decimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
int scn_ulonglong(const char *src, unsigned long long int *dst);

/*!
 * @brief write integer to string in hexadecimal notation
 *
 * @param[out] dest destination string
 * @param[in]  i    integer to write
 *
 * @return number of bytes written to dest
 */
int scn_xlonglong(const char *src, unsigned long long int *dst);

#endif

/*! @} fmt */
