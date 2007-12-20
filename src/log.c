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

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#include "log.h"
#include "cext.h"
#include "printf.h"
#include "str.h"

#define MASK_PRIO(p) (1 << (p))

log_options_t *_log_options = NULL;

static int errno_orig = 0;

static
int mask_to_syslog(int mask)
{
	int newmask = 0;

	if (mask & MASK_PRIO(LOGP_ALERT))
		newmask |= MASK_PRIO(LOG_ALERT);
	if (mask & MASK_PRIO(LOGP_ERROR))
		newmask |= MASK_PRIO(LOG_ERR);
	if (mask & MASK_PRIO(LOGP_WARN))
		newmask |= MASK_PRIO(LOG_WARNING);
	if (mask & MASK_PRIO(LOGP_NOTE))
		newmask |= MASK_PRIO(LOG_NOTICE);
	if (mask & MASK_PRIO(LOGP_INFO))
		newmask |= MASK_PRIO(LOG_INFO);
	if (mask & MASK_PRIO(LOGP_DEBUG))
		newmask |= MASK_PRIO(LOG_DEBUG);

	return newmask;
}

static
int prio_to_syslog(int prio)
{
	switch (prio) {
		case LOGP_ALERT: return LOG_ALERT;
		case LOGP_ERROR: return LOG_ERR;
		case LOGP_WARN:  return LOG_WARNING;
		case LOGP_NOTE:  return LOG_NOTICE;
		case LOGP_INFO:  return LOG_INFO;
		case LOGP_DEBUG: return LOG_DEBUG;
	}

	return -1;
}

void log_init(log_options_t *options)
{
	struct stat sb;

	/* check file destination */
	if (options->log_dest & LOGD_FILE)
		if (options->log_fd < 0 || fstat(options->log_fd, &sb) == -1)
			options->log_dest &= ~LOGD_FILE;

	/* check if STDERR is available */
	if (options->log_dest & LOGD_STDERR)
		if (fstat(STDERR_FILENO, &sb) == -1)
			options->log_dest &= ~LOGD_STDERR;

	/* log up to LOGP_INFO if not specified */
	if (options->log_mask == 0)
		options->log_mask = ((1 << ((LOGP_INFO) + 1)) - 1);

	/* sanitize ident string */
	if (str_isempty(options->log_ident))
		options->log_ident = "(none)";

	if (options->log_dest & LOGD_SYSLOG) {
		openlog(options->log_ident,
				options->log_opts & LOGO_PID ? LOG_PID : 0,
				options->log_facility);
		setlogmask(mask_to_syslog(options->log_mask));
	}

	_log_options = (log_options_t *) malloc(sizeof(log_options_t));

	memcpy(_log_options, options, sizeof(log_options_t));
}

void log_close(void)
{
	if (!_log_options)
		return;

	if (_log_options->log_dest & LOGD_SYSLOG)
		closelog();

	if (_log_options->log_dest & LOGD_FILE)
		close(_log_options->log_fd);

	free(_log_options);

	_log_options = 0;
}

static
void log_fd(int fd, int prio, const char *msg)
{
	char timebuf[17];
	time_t curtime = time(0);

	if (!(_log_options->log_mask & (1 << prio)))
		return;

	if (_log_options->log_opts & LOGO_PRIO) {
		switch (prio) {
			case LOGP_ALERT: _lucid_dprintf(fd, "[alert]"); break;
			case LOGP_ERROR: _lucid_dprintf(fd, "[error]"); break;
			case LOGP_WARN:  _lucid_dprintf(fd, "[warn ]"); break;
			case LOGP_NOTE:  _lucid_dprintf(fd, "[note ]"); break;
			case LOGP_INFO:  _lucid_dprintf(fd, "[info ]"); break;
			case LOGP_DEBUG: _lucid_dprintf(fd, "[debug]"); break;
			case LOGP_TRACE: _lucid_dprintf(fd, "[trace]"); break;
			default:         _lucid_dprintf(fd, "[none ]"); break;
		}
	}

	if (_log_options->log_opts & LOGO_TIME) {
		memset(timebuf, 0, 17);
		strftime(timebuf, 17, "%b %d %T", localtime(&curtime));
		_lucid_dprintf(fd, " %s", timebuf);
	}

	if (_log_options->log_opts & LOGO_IDENT)
		_lucid_dprintf(fd, " %s", _log_options->log_ident);

	if (_log_options->log_opts & LOGO_PID)
		_lucid_dprintf(fd, "[%d]", getpid());

	_lucid_dprintf(fd, "%s%s\n",
			_log_options->log_opts ? ": " : "", msg);
}

static
void log_internal(int prio, int strerr, const char *fmt, va_list ap)
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

	if (_log_options->log_dest & LOGD_STDERR)
		log_fd(STDERR_FILENO, prio, msg);

	if (_log_options->log_dest & LOGD_FILE)
		log_fd(_log_options->log_fd, prio, msg);

	if (_log_options->log_dest & LOGD_SYSLOG) {
		if ((prio = prio_to_syslog(prio)) >= 0)
			syslog(_log_options->log_facility|prio, "%s", msg);
	}

	free(msg);
}

int log_traceme(const char *file, const char *func, int line)
{
	char *base = str_path_basename(file);

	int rc = log_trace("@%s() in %s:%d", func, base, line);

	free(base);

	return rc;
}

#define LOGFUNC(name, level, rc) \
int log_ ## name (const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 0, fmt, ap); \
	va_end(ap); \
	return rc; \
}

LOGFUNC(alert,  LOGP_ALERT, EXIT_FAILURE)
LOGFUNC(error,  LOGP_ERROR, EXIT_FAILURE)
LOGFUNC(warn,   LOGP_WARN,  EXIT_SUCCESS)
LOGFUNC(notice, LOGP_NOTE,  EXIT_SUCCESS)
LOGFUNC(info,   LOGP_INFO,  EXIT_SUCCESS)
LOGFUNC(debug,  LOGP_DEBUG, EXIT_SUCCESS)
LOGFUNC(trace,  LOGP_TRACE, EXIT_SUCCESS)

#define LOGFUNCDIE(name, level) \
void log_ ## name ## _and_die(const char *fmt, ...) { \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 0, fmt, ap); \
	va_end(ap); \
	exit(EXIT_FAILURE); \
}

LOGFUNCDIE(alert, LOGP_ALERT)
LOGFUNCDIE(error, LOGP_ERROR)

#define LOGPFUNC(name, level, rc) \
int log_p ## name (const char *fmt, ...) { \
	errno_orig = errno; \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 1, fmt, ap); \
	va_end(ap); \
	return rc; \
}

LOGPFUNC(alert,  LOGP_ALERT, EXIT_FAILURE)
LOGPFUNC(error,  LOGP_ERROR, EXIT_FAILURE)
LOGPFUNC(warn,   LOGP_WARN,  EXIT_SUCCESS)
LOGPFUNC(notice, LOGP_NOTE,  EXIT_SUCCESS)
LOGPFUNC(info,   LOGP_INFO,  EXIT_SUCCESS)
LOGPFUNC(debug,  LOGP_DEBUG, EXIT_SUCCESS)
LOGPFUNC(trace,  LOGP_TRACE, EXIT_SUCCESS)

#define LOGPFUNCDIE(name, level) \
void log_p ## name ## _and_die(const char *fmt, ...) { \
	errno_orig = errno; \
	va_list ap; va_start(ap, fmt); \
	log_internal(level, 1, fmt, ap); \
	va_end(ap); \
	exit(EXIT_FAILURE); \
}

LOGPFUNCDIE(alert, LOGP_ALERT)
LOGPFUNCDIE(error, LOGP_ERROR)
