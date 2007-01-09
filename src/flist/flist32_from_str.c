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

#include "char.h"
#include "flist.h"
#include "str.h"

int flist32_from_str(const char *str, const flist32_t list[],
                  uint32_t *flags, uint32_t *mask,
                  char clmod, char delim)
{
	char *p, *o, *buf;
	int clear = 0;
	uint32_t cur_flag;
	
	if (str_isempty(str))
		return errno = EINVAL, -1;
	
	buf = p = o = str_dup(str);
	
	while (1) {
		p = str_chr(p, delim, str_len(p));
		
		while (char_isspace(*o))
			o++;
		
		if (p) {
			if (o == p) {
				p++;
				continue;
			}
			
			*p++ = '\0';
		}
		
		if (*o == clmod)
			clear = 1;
		
		cur_flag = flist32_getval(list, o+clear);
		
		if (!cur_flag) {
			free(buf);
			return errno = ENOENT, -1;
		}
		
		if (clear) {
			*flags &= ~cur_flag;
			*mask  |=  cur_flag;
		} else {
			*flags |=  cur_flag;
			*mask  |=  cur_flag;
		}
		
		if (!p)
			break;
		else
			o = p;
	}
	
	free(buf);
	return 0;
}
