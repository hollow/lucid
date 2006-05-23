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

int http_get_request (void *src, http_request_t *request,
                      http_header_t *headers, char **body, http_read_t cb)
{
	char *line;
	struct list_head *pos, *q;
	
	/* 1) read request line */
	for (line = NULL;;) {
		if (line)
			free(line);
		
		line = http_read_line(src, cb);
		
		if (!line)
			return errno = EIO, -1;
		
		if (strlen(line) == 0)
			continue;
		
		break;
	}
	
	char *meth = calloc(sizeof(char *), strlen(line));
	char *uri  = calloc(sizeof(char *), strlen(line));
	unsigned int major, minor;
	
	if (sscanf(line, "%s %s HTTP/%u.%u", meth, uri, &major, &minor) != 4) {
		free(meth);
		free(uri);
		free(line);
		return errno = EPROTO, -1;
	}
	
	free(line);
	
	request->method = http_str_to_method(meth);
	
	bzero(request->uri, HTTP_MAX_URI);
	strncpy(request->uri, uri, HTTP_MAX_URI - 1);
	
	request->vmajor = major;
	request->vminor = minor;
	
	free(meth);
	free(uri);
	
	/* 2) read headers */
	struct list_head *list = &(headers->list);
	http_header_t *tmp;
	char *key, *val;
	size_t clen = 0;
	
	for (line = NULL;;) {
		if (line)
			free(line);
		
		line = http_read_line(src, cb);
		
		if (!line) {
			errno = EIO;
			goto err;
		}
		
		if (strlen(line) == 0)
			break;
		
		key = line;
		val = strchr(key, ':');
		
		*val = '\0';
		val += 2;
		
		if (!val || strlen(key) == 0 || strlen(val) == 0) {
			errno = EPROTO;
			goto err2;
		}
		
		if (strcmp(key, "Content-Length") == 0)
			clen = atoi(val);
		
		tmp = (http_header_t *) malloc(sizeof(http_header_t));
		tmp->key = strdup(key);
		tmp->val = strdup(val);
		
		list_add(&(tmp->list), list);
	}
	
	free(line);
	
	/* 3) read body */
	if (clen > 0) {
		line = http_read_len(src, clen, cb);
		
		if (!line)
			goto err;
		
		if (strlen(line) != clen) {
			errno = EIO;
			goto err2;
		}
	}
	
	*body = line;
	return 0;
	
err2:
	free(line);
err:
	http_headers_free(headers);
	return -1;
}
