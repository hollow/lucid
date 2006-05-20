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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

#include "argv/argv.h"
#include "exec/exec.h"

int exec_replace(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	
	char *cmd;
	vasprintf(&cmd, fmt, ap);
	
	va_end(ap);
	
	int argc;
	char *argv[32];
	
	argc = argv_from_str(cmd, argv, 32);
	
	if (argc < 1)
		return errno = EINVAL, -1;
	
	pid_t pid;
	int i, status;
	
	if (execvp(argv[0], argv) == -1)
		return -1;
	
	/* never get here */
	return 0;
}
