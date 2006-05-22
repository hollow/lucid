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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "http/http.h"
#include "list/list.h"
#include "stralloc/stralloc.h"

int http_send_response(void *dst, http_response_t *response,
                       http_header_t *headers, char *body, http_write_t cb)
{
	char *line;
	char *reason = http_status_to_str(response->status);
	
	STRALLOC buf;
	http_header_t *tmp;
	
	if (!reason)
		return errno = EINVAL, -1;
	
	stralloc_init(&buf);
	
	asprintf(&line, "HTTP/%u.%u %u %s\r\n",
	         response->vmajor, response->vminor, response->status, reason);
	
	stralloc_cats(&buf, line);
	free(line);
	
	list_for_each_entry(tmp, &(headers->list), list) {
		stralloc_cats(&buf, tmp->key);
		stralloc_cats(&buf, ": ");
		stralloc_cats(&buf, tmp->val);
		stralloc_cats(&buf, "\r\n");
	}
	
	stralloc_cats(&buf, "\r\n");
	
	if (body)
		stralloc_cats(&buf, body);
	
	if (cb(dst, buf.s, buf.len) != buf.len) {
		stralloc_free(&buf);
		return errno = EIO, -1;
	}
	
	stralloc_free(&buf);
	return 0;
}
