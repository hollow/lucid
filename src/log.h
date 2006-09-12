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
 * @defgroup log Logging functions
 * @{
 */

#ifndef _LUCID_LOG_H

#include <stdbool.h>
#include <stdarg.h>

typedef struct {
	char *ident;
	bool  file;
	int   fd;
	bool  stderr;
	bool  syslog;
	int   flags;
	int   facility;
	int   mask;
} log_options_t;

void log_init(log_options_t *options);

void log_internal(int level, const char *fmt, va_list ap);

void log_emerg (const char *fmt, ...);
void log_alert (const char *fmt, ...);
void log_crit  (const char *fmt, ...);
void log_error (const char *fmt, ...);
void log_warn  (const char *fmt, ...);
void log_notice(const char *fmt, ...);
void log_debug (const char *fmt, ...);

void log_emerg_and_die(const char *fmt, ...);
void log_alert_and_die(const char *fmt, ...);
void log_crit_and_die (const char *fmt, ...);
void log_error_and_die(const char *fmt, ...);

void log_close(void);

#define _LUCID_LOG_H

#endif

/*! @} log */
