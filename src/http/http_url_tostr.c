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
#include "printf.h"
#include "str.h"

int http_url_tostr(http_url_t *url, char **str)
{
	char *buf;
	int rc =  _lucid_asprintf(&buf, "http://%s:%d%s",
	                          url->host, url->port, url->path);
	
	if (rc == -1)
		return -1;
	
	*str = buf;
	
	if (!str_isempty(url->query)) {
		rc = _lucid_asprintf(&buf, "%s?%s", *str, url->query);
		
		if (rc == -1) {
			mem_free(*str);
			return -1;
		}
		
		mem_free(*str);
		*str = buf;
	}
	
	return 0;
}
