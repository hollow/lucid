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

#ifndef _LUCID_HTTP_H
#define _LUCID_HTTP_H

#include "list/list.h"

typedef enum {
	HTTP_METHOD_INVALID = 0,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_GET,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_CONNECT
} http_method_t;

typedef enum {
	HTTP_STATUS_INVALID   = 0,
	HTTP_STATUS_CONTINUE  = 100,
	HTTP_STATUS_SWITCHING = 101,
	HTTP_STATUS_OK        = 200,
	HTTP_STATUS_CREATED   = 201,
	HTTP_STATUS_ACCEPTED  = 202,
	HTTP_STATUS_NONAUTH   = 203,
	HTTP_STATUS_NOCONTENT = 204,
	HTTP_STATUS_RESET     = 205,
	HTTP_STATUS_PARTIAL   = 206,
	HTTP_STATUS_MULTIPLE  = 300,
	HTTP_STATUS_MOVED     = 301,
	HTTP_STATUS_FOUND     = 302,
	HTTP_STATUS_OTHER     = 303,
	HTTP_STATUS_NOMOD     = 304,
	HTTP_STATUS_PROXY     = 305,
	HTTP_STATUS_REDIRECT  = 307,
	HTTP_STATUS_BADREQ    = 400,
	HTTP_STATUS_UNAUTH    = 401,
	HTTP_STATUS_PAYMENT   = 402,
	HTTP_STATUS_FORBIDDEN = 403,
	HTTP_STATUS_NOTFOUND  = 404,
	HTTP_STATUS_ALLOWED   = 405,
	HTTP_STATUS_NOACCEPT  = 406,
	HTTP_STATUS_PROXYAUTH = 407,
	HTTP_STATUS_TIMEOUT   = 408,
	HTTP_STATUS_CONFLICT  = 409,
	HTTP_STATUS_GONE      = 410,
	HTTP_STATUS_LENGTH    = 411,
	HTTP_STATUS_PRECON    = 412,
	HTTP_STATUS_ENTBIG    = 413,
	HTTP_STATUS_REQBIG    = 414,
	HTTP_STATUS_MEDIA     = 415,
	HTTP_STATUS_RANGE     = 416,
	HTTP_STATUS_EXPECT    = 417,
	HTTP_STATUS_INTERNAL  = 500,
	HTTP_STATUS_NOIMPL    = 501,
	HTTP_STATUS_GATEWAY   = 502,
	HTTP_STATUS_SERVICE   = 503,
	HTTP_STATUS_GTIMEOUT  = 504,
	HTTP_STATUS_VERSION   = 505,
} http_status_t;

typedef struct {
	struct list_head list;
	char *key;
	char *val;
} http_header_t;

#define HTTP_MAX_URI 128

typedef struct {
	http_method_t method;
	char uri[HTTP_MAX_URI];
	unsigned int vmajor;
	unsigned int vminor;
} http_request_t;

typedef struct {
	http_status_t status;
	unsigned int vmajor;
	unsigned int vminor;
} http_response_t;

typedef size_t (*http_read_t) (const void *, char *, size_t);
typedef size_t (*http_write_t)(const void *, const char *, size_t);

const char *http_method_to_str(http_method_t method);
const char *http_status_to_str(http_status_t status);

http_method_t http_str_to_method(const char *str);
http_status_t http_str_to_status(const char *str);

int http_get_request (const void *src, http_request_t *request,
                      http_header_t *headers, char **body, http_read_t cb);
int http_get_response(const void *src, http_response_t *response,
                      http_header_t *headers, char **body, http_read_t cb);

int http_send_request (const void *dst, const http_request_t *request,
                       const http_header_t *headers, const char *body,
                       http_write_t cb);
int http_send_response(const void *dst, const http_response_t *response,
                       const http_header_t *headers, const char *body,
                       http_write_t cb);

size_t http_read_callback (const void *src, char *data, size_t len);
size_t http_write_callback(const void *dst, const char *data, size_t len);

void http_headers_free(http_header_t *headers);

/* private interface */
struct _http_method {
	http_method_t method;
	const char *name;
};

struct _http_status {
	http_status_t status;
	const char *desc;
};

extern const struct _http_method _http_method_map[];
extern const struct _http_status _http_status_map[];

char *http_read_line(const void *src, http_read_t cb);
char *http_read_len(const void *src, size_t len, http_read_t cb);

#endif
