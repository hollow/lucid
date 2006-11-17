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
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>

#include "log.h"

log_options_t *_log_options = NULL;

void log_init(log_options_t *options)
{
	struct stat sb;
	
	if (options->file && (options->fd < 0 || fstat(options->fd, &sb) == -1))
		options->file = false;
	
	if (options->stderr && fstat(STDERR_FILENO, &sb) == -1)
		options->stderr = false;
	
	if (options->mask == 0)
		options->mask = LOG_UPTO(LOG_INFO);
	
	if (!options->ident || strlen(options->ident) < 1)
		options->ident = "(none)";
	
	options->flags &= ~LOG_PERROR;
	
	if (options->syslog) {
		openlog(options->ident, options->flags, options->facility);
		setlogmask(options->mask);
	}
	
	_log_options = (log_options_t *)malloc(sizeof(log_options_t));
	
	memcpy(_log_options, options, sizeof(log_options_t));
}
