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
#include "mem.h"
#include "scanf.h"
#include "str.h"
#include "strtok.h"

int http_request_fromstr(const char *str, http_request_t *request)
{
	int rc = 0;
	char *scpy, *cur, *reqline;
	strtok_t _st, *st = &_st, *p;
	
	scpy = cur = reqline = str_dup(str);
	
	/* get request line */
	cur = str_str(cur, "\r\n");
	
	if (!cur) {
		mem_free(scpy);
		return errno = EINVAL, -1;
	}
	
	mem_set(cur, 0, 2);
	cur += 2;
	
	/* parse request method */
	char *method = reqline;
	
	reqline = str_chr(method, ' ', str_len(method));
	
	if (!reqline) {
		mem_free(scpy);
		return errno = EINVAL, -1;
	}
	
	*reqline++ = '\0';
	
	if ((request->method = str_to_method(method)) == HTTP_METHOD_UNKNOWN) {
		mem_free(scpy);
		return errno = EINVAL, -1;
	}
	
	/* parse request path */
	char *path = reqline;
	
	reqline = str_chr(path, ' ', str_len(path));
	
	if (!reqline) {
		mem_free(scpy);
		return errno = EINVAL, -1;
	}
	
	*reqline++ = '\0';
	
	if (!str_path_isabs(path)) {
		mem_free(scpy);
		return errno = EINVAL, -1;
	}
	
	request->path = str_dup(path);
	
	/* parse http version */
	if (_lucid_sscanf(reqline, "HTTP/%d.%d",
	                  &(request->major), &(request->minor)) != 2) {
		mem_free(scpy);
		return errno = EINVAL, -1;
	}
	
	/* get headers */
	if (!strtok_init_str(st, cur, "\r\n", 0)) {
		mem_free(request->path);
		mem_free(scpy);
		goto free;
	}
	
	mem_free(scpy);
	
	http_header_t *hs = &(request->headers);
	http_header_init(hs);
	
	http_header_name_t name;
	char *value;
	
	strtok_for_each(st, p) {
		value = str_chr(p->token, ':', str_len(p->token));
		
		if (!value) {
			errno = EINVAL;
			goto free;
		}
		
		*value++ = '\0';
		
		name = str_to_headername(p->token);
		
		if (name == HTTP_HEADER_UNKNOWN) {
			errno = EINVAL;
			goto free;
		}
		
		if (http_header_add(hs, name, value) == -1)
			goto free;
	}
	
	goto out;
	
free:
	mem_free(request->path);
	http_header_free(hs);
	rc = -1;
	
out:
	strtok_free(st);
	return rc;
}
