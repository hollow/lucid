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
#include <syslog.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "log.h"
#include "printf.h"
#include "str.h"

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
		case LOG_EMERG:   _lucid_dprintf(fd, "[emrge]"); break;
		case LOG_ALERT:   _lucid_dprintf(fd, "[alert]"); break;
		case LOG_CRIT:    _lucid_dprintf(fd, "[crit ]"); break;
		case LOG_ERR:     _lucid_dprintf(fd, "[error]"); break;
		case LOG_WARNING: _lucid_dprintf(fd, "[warn ]"); break;
		case LOG_NOTICE:  _lucid_dprintf(fd, "[note ]"); break;
		case LOG_INFO:    _lucid_dprintf(fd, "[info ]"); break;
		case LOG_DEBUG:   _lucid_dprintf(fd, "[debug]"); break;
		default:          _lucid_dprintf(fd, "[none ]"); break;
	}
	
	if (_log_options->time) {
		str_zero(timebuf, 17);
		strftime(timebuf, 17, "%b %d %T", localtime(&curtime));
		_lucid_dprintf(fd, " %s", timebuf);
	}
	
	_lucid_dprintf(fd, " %s", _log_options->ident);
	
	if (_log_options->flags & LOG_PID)
		_lucid_dprintf(fd, "[%d]", getpid());
	
	_lucid_dprintf(fd, ": %s\n", msg);
}

void log_internal(int level, int strerr, const char *fmt, va_list ap)
{
	char *buf, *msg;
	
	if (!_log_options)
		return;
	
	_lucid_vasprintf(&msg, fmt, ap);
	
	if (strerr) {
		buf = msg;
		_lucid_asprintf(&msg, "%s: %s", buf, strerror(errno_orig));
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

#define LOGFUNC(name, level, rc) \
int log_ ## name (const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 0, fmt, ap); \
	va_end(ap); \
	return rc; \
}

LOGFUNC(emerg,  LOG_EMERG,   EXIT_FAILURE)
LOGFUNC(alert,  LOG_ALERT,   EXIT_FAILURE)
LOGFUNC(crit,   LOG_CRIT,    EXIT_FAILURE)
LOGFUNC(error,  LOG_ERR,     EXIT_FAILURE)
LOGFUNC(warn,   LOG_WARNING, EXIT_SUCCESS)
LOGFUNC(notice, LOG_NOTICE,  EXIT_SUCCESS)
LOGFUNC(info,   LOG_INFO,    EXIT_SUCCESS)
LOGFUNC(debug,  LOG_DEBUG,   EXIT_SUCCESS)

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

#define LOGPFUNC(name, level, rc) \
int log_p ## name (const char *fmt, ...) { \
	errno_orig = errno; \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 1, fmt, ap); \
	va_end(ap); \
	return rc; \
}

LOGPFUNC(emerg,  LOG_EMERG,   EXIT_FAILURE)
LOGPFUNC(alert,  LOG_ALERT,   EXIT_FAILURE)
LOGPFUNC(crit,   LOG_CRIT,    EXIT_FAILURE)
LOGPFUNC(error,  LOG_ERR,     EXIT_FAILURE)
LOGPFUNC(warn,   LOG_WARNING, EXIT_SUCCESS)
LOGPFUNC(notice, LOG_NOTICE,  EXIT_SUCCESS)
LOGPFUNC(info,   LOG_INFO,    EXIT_SUCCESS)
LOGPFUNC(debug,  LOG_DEBUG,   EXIT_SUCCESS)

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

