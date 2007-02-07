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
#include <syslog.h>
#include <sys/stat.h>

#include "log.h"
#include "mem.h"
#include "str.h"

log_options_t *_log_options = 0;

#define MASK_PRIO(p) (1 << (p))

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

	_log_options = (log_options_t *) mem_alloc(sizeof(log_options_t));

	mem_cpy(_log_options, options, sizeof(log_options_t));
}
