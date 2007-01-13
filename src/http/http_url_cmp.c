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
#include "str.h"

int http_url_cmp(http_url_t *a, http_url_t *b)
{
	int rc = str_cmp(str_tolower(a->host), str_tolower(b->host));
	
	if (rc != 0)
		return rc;
	
	rc = a->port - b->port;
	
	if (rc != 0)
		return rc;
	
	return str_cmp(a->path, b->path);
}
