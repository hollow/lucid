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

#include "flist.h"
#include "strtok.h"

int flist64_from_str(const char *str, const flist64_t list[],
                  uint64_t *flags, uint64_t *mask,
                  char clmod, char *delim)
{
	char *token;
	int clear = 0;
	uint64_t cur_flag;
	
	strtok_t _st, *st = &_st, *p;
	
	if (!strtok_init_str(st, str, delim, 0))
		return -1;
	
	strtok_for_each(st, p) {
		token = p->token;
		
		if (*token == clmod)
			clear = 1;
		
		cur_flag = flist64_getval(list, token+clear);
		
		if (!cur_flag) {
			strtok_free(st);
			return -1;
		}
		
		if (clear) {
			*flags &= ~cur_flag;
			*mask  |=  cur_flag;
		} else {
			*flags |=  cur_flag;
			*mask  |=  cur_flag;
		}
	}
	
	strtok_free(st);
	
	return 0;
}
