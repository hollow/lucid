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

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "argv/argv.h"

int argv_parse(char *str, int *argc, char ***argv)
{
	int ac = 1;
	char **av;
	
	if (strlen(str) < 1) {
		errno = EINVAL;
		return -1;
	}
	
	char *p;
	
	/* count number of arguments */
	for (p = str; (p = strchr(p, ' ')); p++)
		ac++;
	
	av = (char **) calloc(ac + 1, sizeof(char *));
	
	if (!av)
		return -1;
	
	int len, i = 0;
	
	for (p = strtok(str, " "); p != NULL && i < ac; i++) {
		len = strlen(p);
		
		av[i] = malloc(len + 1);
		
		if (!av[i])
			goto err;
		
		memset(av[i], '\0', len + 1);
		memcpy(av[i], p, len);
		
		p = strtok(NULL, " ");
	}
	
	*argc = ac;
	*argv = av;
	
	return 0;
	
err:
	while (i > 0)
		free(av[i--]);
	
	free(av);
	return -1;
}
