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
 * @defgroup scanf Formatted input conversion
 * @{
 */

#ifndef _LUCID_SCANF_H
#define _LUCID_SCANF_H

#include <stdarg.h>

/*!
 * @brief read conversion from string using va_list
 *
 * @param[in]  str  source string
 * @param[in]  fmt  format string
 * @param[out] ap   variable number of arguments
 *
 * @return Number of converted arguments
 *
 * @note Every conversion happens in this functions. All other scanf functions
 *       are just convenient wrappers.
 */
int _lucid_vsscanf(const char *str, const char *fmt, va_list ap);

/*!
 * @brief read conversion from string using variable number of arguments
 *
 * @param[in]  str  source string
 * @param[in]  fmt  format string
 * @param[out] ...  variable number of arguments
 *
 * @return Number of converted arguments
 */
int _lucid_sscanf(const char *str, const char *fmt, /*args*/ ...);

#endif

/*! @} printf */
