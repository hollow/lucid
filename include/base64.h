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
 * @defgroup base64 base64 encoding/decoding functions
 * @{
 */

#ifndef _LUCID_BASE64_H
#define _LUCID_BASE64_H

#include <sys/types.h>

#define BASE64_LENGTH(N) ((((N) + 2) / 3) * 4)

char *base64_encode(const void *data, size_t n);
void *base64_decode(const char *buf);

#endif

/*! @} base64 */
