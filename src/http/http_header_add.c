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

#include "http.h"
#include "mem.h"
#include "str.h"

int http_header_add(http_header_t *hs, http_header_name_t name, const char *value)
{
	http_header_t *new = mem_alloc(sizeof(http_header_t));
	
	if (!new)
		return -1;
	
	new->name  = name;
	new->value = str_dup(value);
	
	if (!new->value)
		return -1;
	
	list_add_tail(&(new->list), &(hs->list));
	
	return 0;
}
