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
#include <errno.h>
#include <signal.h>

#include "exec.h"
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
	
	strtok_t st;
	
	if (!strtok_init_str(&st, cmd, ' ', 0)) {
		free(cmd);
		return errno = ENOMEM, -1;
	}
	
	free(cmd);
	
	int argc = strtok_count(&st);
	char **argv = calloc(argc + 1, sizeof(char *));
	
	if (strtok_toargv(&st, argv) < 1) {
		strtok_free(&st);
		return errno = EINVAL, -1;
	}
	
	pid_t pid;
	int i;
	
	switch ((pid = fork())) {
	case -1:
		return -1;
	
	case 0:
		usleep(200);
		
		for (i = 0; i < 100; i++)
			close(i);
		
		execvp(argv[0], argv);
	
	default:
		signal(SIGCHLD, SIG_IGN);
	}
	
	free(argv);
	strtok_free(&st);
	return 0;
}
