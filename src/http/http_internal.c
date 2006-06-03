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
#include <string.h>

#include "http/http.h"
#include "io/io.h"

const struct _http_method _http_method_map[] = {
	{ HTTP_METHOD_OPTIONS, "OPTIONS" },
	{ HTTP_METHOD_GET,     "GET" },
	{ HTTP_METHOD_HEAD,    "HEAD" },
	{ HTTP_METHOD_POST,    "POST" },
	{ HTTP_METHOD_PUT,     "PUT" },
	{ HTTP_METHOD_DELETE,  "DELETE" },
	{ HTTP_METHOD_TRACE,   "TRACE" },
	{ HTTP_METHOD_CONNECT, "CONNECT" },
	{ 0, NULL }
};

const struct _http_status _http_status_map[] = {
	{ HTTP_STATUS_CONTINUE,  "Continue" },
	{ HTTP_STATUS_SWITCHING, "Switching Protocols" },
	{ HTTP_STATUS_OK,        "OK" },
	{ HTTP_STATUS_CREATED,   "Created" },
	{ HTTP_STATUS_ACCEPTED,  "Accepted" },
	{ HTTP_STATUS_NONAUTH,   "Non-Authoritative Information" },
	{ HTTP_STATUS_NOCONTENT, "No Content" },
	{ HTTP_STATUS_RESET,     "Reset Content" },
	{ HTTP_STATUS_PARTIAL,   "Partial Content" },
	{ HTTP_STATUS_MULTIPLE,  "Multiple Choices" },
	{ HTTP_STATUS_MOVED,     "Moved Permanently" },
	{ HTTP_STATUS_FOUND,     "Found" },
	{ HTTP_STATUS_OTHER,     "See Other" },
	{ HTTP_STATUS_NOMOD,     "Not Modified" },
	{ HTTP_STATUS_PROXY,     "Use Proxy" },
	{ HTTP_STATUS_REDIRECT,  "Temporary Redirect" },
	{ HTTP_STATUS_BADREQ,    "Bad Request" },
	{ HTTP_STATUS_UNAUTH,    "Unauthorized" },
	{ HTTP_STATUS_PAYMENT,   "Payment Required" },
	{ HTTP_STATUS_FORBIDDEN, "Forbidden" },
	{ HTTP_STATUS_NOTFOUND,  "Not Found" },
	{ HTTP_STATUS_ALLOWED,   "Method Not Allowed" },
	{ HTTP_STATUS_NOACCEPT,  "Not Acceptable" },
	{ HTTP_STATUS_PROXYAUTH, "Proxy Authentication Required" },
	{ HTTP_STATUS_TIMEOUT,   "Request Timeout" },
	{ HTTP_STATUS_CONFLICT,  "Conflict" },
	{ HTTP_STATUS_GONE,      "Gone" },
	{ HTTP_STATUS_LENGTH,    "Length Required" },
	{ HTTP_STATUS_PRECON,    "Precondition Failed" },
	{ HTTP_STATUS_ENTBIG,    "Request Entity Too Large" },
	{ HTTP_STATUS_REQBIG,    "Request-URI Too Long" },
	{ HTTP_STATUS_MEDIA,     "Unsupported Media Type" },
	{ HTTP_STATUS_RANGE,     "Requested Range Not Satisfiable" },
	{ HTTP_STATUS_EXPECT,    "Expectation Failed" },
	{ HTTP_STATUS_INTERNAL,  "Internal Server Error" },
	{ HTTP_STATUS_NOIMPL,    "Not Implemented" },
	{ HTTP_STATUS_GATEWAY,   "Bad Gateway" },
	{ HTTP_STATUS_SERVICE,   "Service Unavailable" },
	{ HTTP_STATUS_GTIMEOUT,  "Gateway Timeout" },
	{ HTTP_STATUS_VERSION,   "HTTP Version Not Supported" },
	{ 0, NULL }
};

void http_headers_free(http_header_t *headers)
{
	struct list_head *pos, *q;
	http_header_t *tmp;
	
	list_for_each_safe(pos, q, &(headers->list)) {
		tmp = list_entry(pos, http_header_t, list);
		list_del(pos);
		free(tmp->key);
		free(tmp->val);
		free(tmp);
	}
}

char *http_read_line(const void *src, http_read_t cb)
{
	int rc;
	size_t chunks = 1, len = 0;
	char *line, *buf = malloc(chunks * CHUNKSIZE + 1);
	char c;
	
	if (!cb)
		cb = http_read_callback;
	
	for (;;) {
		rc = cb(src, &c, 1);
		
		if (rc < 1) {
			free(buf);
			return NULL;
		}
		
		if (c == '\r')
			continue;
		
		if (c == '\n')
			goto out;
		
		if (len >= chunks * CHUNKSIZE) {
			chunks++;
			buf = realloc(buf, chunks * CHUNKSIZE + 1);
		}
		
		buf[len++] = c;
	}
	
out:
	line = strndup(buf, len);
	free(buf);
	return line;
}

char *http_read_len(const void *src, size_t len, http_read_t cb)
{
	char *buf = malloc(len + 1);
	bzero(buf, len + 1);
	
	if (!cb)
		cb = http_read_callback;
	
	if (cb(src, buf, len) != len) {
		free(buf);
		return NULL;
	}
	
	return buf;
}
