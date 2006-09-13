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
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

#include "log.h"

extern log_options_t *_log_options;

static int errno_orig = 0;

static
void log_fd(int fd, int level, const char *msg)
{
	char timebuf[17];
	time_t curtime = time(NULL);
	
	if (!(_log_options->mask & level))
		return;
	
	switch (level) {
		case LOG_EMERG:   dprintf(fd, "[emrge]"); break;
		case LOG_ALERT:   dprintf(fd, "[alert]"); break;
		case LOG_CRIT:    dprintf(fd, "[crit ]"); break;
		case LOG_ERR:     dprintf(fd, "[error]"); break;
		case LOG_WARNING: dprintf(fd, "[warn ]"); break;
		case LOG_NOTICE:  dprintf(fd, "[note ]"); break;
		case LOG_INFO:    dprintf(fd, "[info ]"); break;
		case LOG_DEBUG:   dprintf(fd, "[debug]"); break;
		default:          dprintf(fd, "[none ]"); break;
	}
	
	if (_log_options->time) {
		bzero(timebuf, 17);
		strftime(timebuf, 17, "%b %d %T", localtime(&curtime));
		dprintf(fd, " %s", timebuf);
	}
	
	dprintf(fd, " %s", _log_options->ident);
	
	if (_log_options->flags & LOG_PID)
		dprintf(fd, "[%d]", getpid());
	
	dprintf(fd, ": %s\n", msg);
}

void log_internal(int level, int strerr, const char *fmt, va_list ap)
{
	char *buf, *msg;
	
	if (!_log_options)
		return;
	
	vasprintf(&msg, fmt, ap);
	
	if (strerr) {
		buf = msg;
		asprintf(&msg, "%s: %s", buf, strerror(errno_orig));
		free(buf);
	}
	
	if (_log_options->syslog)
		syslog(_log_options->facility|level, "%s", msg);
	
	if (_log_options->stderr)
		log_fd(STDERR_FILENO, level, msg);
	
	if (_log_options->file)
		log_fd(_log_options->fd, level, msg);
	
	free(msg);
}

#define LOGFUNC(name, level) \
void log_ ## name (const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 0, fmt, ap); \
	va_end(ap); \
}

LOGFUNC(emerg,  LOG_EMERG)
LOGFUNC(alert,  LOG_ALERT)
LOGFUNC(crit,   LOG_CRIT)
LOGFUNC(error,  LOG_ERR)
LOGFUNC(warn,   LOG_WARNING)
LOGFUNC(notice, LOG_NOTICE)
LOGFUNC(info,   LOG_INFO)
LOGFUNC(debug,  LOG_DEBUG)

#define LOGFUNCDIE(name, level) \
void log_ ## name ## _and_die(const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 0, fmt, ap); \
	va_end(ap); \
	exit(EXIT_FAILURE); \
}

LOGFUNCDIE(emerg, LOG_EMERG)
LOGFUNCDIE(alert, LOG_ALERT)
LOGFUNCDIE(crit,  LOG_CRIT)
LOGFUNCDIE(error, LOG_ERR)

#define LOGPFUNC(name, level) \
void log_p ## name (const char *fmt, ...) { \
	errno_orig = errno; \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 1, fmt, ap); \
	va_end(ap); \
}

LOGPFUNC(emerg,  LOG_EMERG)
LOGPFUNC(alert,  LOG_ALERT)
LOGPFUNC(crit,   LOG_CRIT)
LOGPFUNC(error,  LOG_ERR)
LOGPFUNC(warn,   LOG_WARNING)
LOGPFUNC(notice, LOG_NOTICE)
LOGPFUNC(info,   LOG_INFO)
LOGPFUNC(debug,  LOG_DEBUG)

#define LOGPFUNCDIE(name, level) \
void log_p ## name ## _and_die(const char *fmt, ...) { \
	errno_orig = errno; \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 1, fmt, ap); \
	va_end(ap); \
	exit(EXIT_FAILURE); \
}

LOGPFUNCDIE(emerg, LOG_EMERG)
LOGPFUNCDIE(alert, LOG_ALERT)
LOGPFUNCDIE(crit,  LOG_CRIT)
LOGPFUNCDIE(error, LOG_ERR)

