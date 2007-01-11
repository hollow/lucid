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

#include <errno.h>

#include "http.h"
#include "mem.h"
#include "str.h"

int http_url_fromstr(const char *str, http_url_t *url)
{
	char *p;
	
	if (str_isempty(str) || !url)
		return errno = EINVAL, -1;
	
	url->port  = 80;
	url->path  = "/";
	url->query = "";
	
	if (str_cmpn(str, "http://", 7) != 0)
		return errno = EINVAL, -1;
	
	str += 7;
	
	url->host = p = str_dup(str);
	
	while (*p && *p != ':' && *p != '/')
		p++, str++;
	
	*p = '\0';
	
	unsigned long long val;
	
	if (*str == ':') {
		str++;
		str += str_toumax(str, &val, 10, str_len(p));
		url->port = (uint16_t) val;
	}
	
	if (*str == '/') {
		url->path = p = str_dup(str);
		
		while (*p && *p != '?')
			p++, str++;
		
		*p = '\0';
		
		if (*str == '?') {
			str++;
			url->query = str_dup(str);
		}
	}
	
	return 0;
}
