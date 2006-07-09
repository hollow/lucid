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
#include <string.h>
#include <errno.h>

#include "flist/flist.h"
#include "str/str.h"

int flist64_from_str(const char *str, const flist64_t list[],
                  uint64_t *flags, uint64_t *mask,
                  char clmod, char delim)
{
	char *p, *o, *buf;
	int clear = 0;
	uint64_t cur_flag;
	
	if (str_isempty(str))
		return errno = EINVAL, -1;
	
	buf = o = strdup(str);
	
	for (p = strtok(buf, &delim); p; p = strtok(NULL, &delim), clear = 0) {
		if (*p == clmod)
			clear = 1;
		
		cur_flag = flist64_getval(list, p+clear);
		
		if (!cur_flag) {
			free(o);
			return errno = ENOENT, -1;
		}
		
		if (clear) {
			*flags &= ~cur_flag;
			*mask  |=  cur_flag;
		} else {
			*flags |=  cur_flag;
			*mask  |=  cur_flag;
		}
	}
	
	free(o);
	return 0;
}
