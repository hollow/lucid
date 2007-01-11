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

#include "mem.h"
#include "log.h"

extern log_options_t *_log_options;

void log_close(void)
{
	if (!_log_options)
		return;
	
	if (_log_options->syslog)
		closelog();
	
	if (_log_options->file)
		close(_log_options->fd);
	
	mem_free(_log_options);
	
	_log_options = 0;
}
