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

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

#include "log.h"

extern log_options_t *_log_options;

static
void log_fd(int fd, int level, const char *fmt, va_list ap)
{
	if (!(_log_options->mask & level))
		return;
	
	switch (level) {
		case LOG_EMERG:   dprintf(fd, "[emerg] "); break;
		case LOG_ALERT:   dprintf(fd, "[alert] "); break;
		case LOG_CRIT:    dprintf(fd, "[crit ] "); break;
		case LOG_ERR:     dprintf(fd, "[error] "); break;
		case LOG_WARNING: dprintf(fd, "[warn ] "); break;
		case LOG_NOTICE:  dprintf(fd, "[note ] "); break;
		case LOG_INFO:    dprintf(fd, "[info ] "); break;
		case LOG_DEBUG:   dprintf(fd, "[debug] "); break;
		default:          dprintf(fd, "[none ] "); break;
	}
	
	dprintf(fd, "%s", _log_options->ident);
	
	if (_log_options->flags & LOG_PID)
		dprintf(fd, "[%d]: ", getpid());
	
	else
		dprintf(fd, ": ");
	
	vdprintf(fd, fmt, ap);
}

void log_internal(int level, const char *fmt, va_list ap)
{
	va_list ap2;
	
	if (!_log_options)
		return;
	
	if (_log_options->syslog) {
		va_copy(ap2, ap);
		vsyslog(_log_options->facility|level, fmt, ap2);
		va_end(ap2);
	}
	
	if (_log_options->stderr) {
		va_copy(ap2, ap);
		log_fd(STDERR_FILENO, level, fmt, ap2);
		va_end(ap2);
	}
	
	if (_log_options->file) {
		va_copy(ap2, ap);
		log_fd(_log_options->fd, level, fmt, ap2);
		va_end(ap2);
	}
}

#define LOGFUNC(name, level) \
void log_ ## name (const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, fmt, ap); \
	va_end(ap); \
}

LOGFUNC(emerg,  LOG_EMERG)
LOGFUNC(alert,  LOG_ALERT)
LOGFUNC(crit,   LOG_CRIT)
LOGFUNC(error,  LOG_ERR)
LOGFUNC(warn,   LOG_WARNING)
LOGFUNC(notice, LOG_NOTICE)
LOGFUNC(debug,  LOG_DEBUG)

#define LOGFUNCDIE(name, level) \
void log_ ## name ## _and_die(const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, fmt, ap); \
	va_end(ap); \
	exit(EXIT_FAILURE); \
}

LOGFUNCDIE(emerg, LOG_EMERG)
LOGFUNCDIE(alert, LOG_ALERT)
LOGFUNCDIE(crit,  LOG_CRIT)
LOGFUNCDIE(error, LOG_ERR)
