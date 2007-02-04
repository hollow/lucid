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
#include <stdarg.h>

#include "exec.h"
#include "mem.h"
#include "printf.h"
#include "strtok.h"

int exec_replace(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	char *cmd;

	if (_lucid_vasprintf(&cmd, fmt, ap) == -1) {
		va_end(ap);
		return -1;
	}

	va_end(ap);

	strtok_t _st, *st = &_st;

	if (!strtok_init_str(st, cmd, " ", 0)) {
		mem_free(cmd);
		return -1;
	}

	mem_free(cmd);

	int argc    = strtok_count(st);
	char **argv = mem_alloc((argc + 1) * sizeof(char *));

	if (!argv) {
		strtok_free(st);
		return -1;
	}

	if (strtok_toargv(st, argv) < 1) {
		mem_free(argv);
		strtok_free(st);
		return -1;
	}

	execvp(argv[0], argv);

	/* never get here */
	mem_free(argv);
	strtok_free(st);
	return -1;
}
