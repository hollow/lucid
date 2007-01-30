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
#include <signal.h>

#include "exec.h"
#include "mem.h"
#include "printf.h"
#include "strtok.h"

int exec_fork_background(const char *fmt, ...)
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
		mem_free(argv);
		strtok_free(st);
		return -1;
	}

	if (strtok_toargv(st, argv) < 1) {
		strtok_free(st);
		return -1;
	}

	pid_t pid;
	int i;

	switch ((pid = fork())) {
	case -1:
		return -1;

	case 0:
		usleep(200);

		strtok_free(st);

		for (i = 0; i < 100; i++)
			close(i);

		execvp(argv[0], argv);

	default:
		signal(SIGCHLD, SIG_IGN);
	}

	mem_free(argv);
	strtok_free(st);
	return 0;
}
