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

#ifndef _LUCID_PRINTF_H
#define _LUCID_PRINTF_H

#include <stdarg.h>
#include <sys/types.h>

int _lucid_vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
int _lucid_snprintf (char *str, size_t size, const char *fmt, /*args*/ ...);
int _lucid_vasprintf(char **ptr, const char *fmt, va_list ap);
int _lucid_asprintf (char **ptr, const char *fmt, /*args*/ ...);
int _lucid_vdprintf (int fd, const char *fmt, va_list ap);
int _lucid_dprintf  (int fd, const char *fmt, /*args*/ ...);
int _lucid_vprintf  (const char *fmt, va_list ap);
int _lucid_printf   (const char *fmt, /*args*/ ...);

#endif
