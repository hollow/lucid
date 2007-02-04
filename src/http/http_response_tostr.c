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
#include "http_internal.h"
#include "stralloc.h"

int http_response_tostr(http_response_t *response, char **str)
{
	stralloc_t _sa, *sa = &_sa;

	stralloc_init(sa);

	const char *status = status_to_str(response->status);

	if (!status) {
		stralloc_free(sa);
		return errno = EINVAL, -1;
	}

	if (stralloc_catf(sa, "HTTP/%d.%d %s %s\r\n", response->major,
	                  response->minor, status, response->reason) == -1) {
		stralloc_free(sa);
		return -1;
	}

	http_header_t *hs = &(response->headers), *p;

	http_header_for_each(hs, p) {
		const char *name = headername_to_str(p->name);

		if (!name) {
			stralloc_free(sa);
			return errno = EINVAL, -1;
		}

		if (stralloc_catf(sa, "%s: %s\r\n", name, p->value) == -1) {
			stralloc_free(sa);
			return -1;
		}
	}

	if (str)
		*str = stralloc_finalize(sa);

	stralloc_free(sa);
	return 0;
}
