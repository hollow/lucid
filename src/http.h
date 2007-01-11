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

/*!
 * @defgroup http HTTP Protocol Parser
 *
 * @{
 */

#ifndef _LUCID_HTTP_H
#define _LUCID_HTTP_H

#include <stdint.h>

#include "list.h"

typedef struct {
	char    *host;
	uint16_t port;
	char    *path;
	char    *query;
} http_url_t;

int http_url_fromstr(const char *str, http_url_t *url);
int http_url_tostr(http_url_t *url, char **str);
int http_url_cmp(http_url_t *a, http_url_t *b);

typedef enum {
	// unknown header (always an error)
	HTTP_HEADER_UNKNOWN,
	
	// general headers
	HTTP_HEADER_CACHE_CONTROL,
	HTTP_HEADER_CONNECTION,
	HTTP_HEADER_DATE,
	HTTP_HEADER_PRAGMA,
	HTTP_HEADER_TRAILER,
	HTTP_HEADER_TRANFSER_ENCODING,
	HTTP_HEADER_UPGRADE,
	HTTP_HEADER_VIA,
	HTTP_HEADER_WARNING,
	
	// request headers
	HTTP_HEADER_ACCEPT,
	HTTP_HEADER_ACCEPT_CHARSET,
	HTTP_HEADER_ACCEPT_ENCODING,
	HTTP_HEADER_ACCEPT_LANGUAGE,
	HTTP_HEADER_AUTHORIZATION,
	HTTP_HEADER_EXPECT,
	HTTP_HEADER_FROM,
	HTTP_HEADER_HOST,
	HTTP_HEADER_IF_MATCH,
	HTTP_HEADER_IF_MODIFIED_SINCE,
	HTTP_HEADER_IF_NONE_MATCH,
	HTTP_HEADER_IF_RANGE,
	HTTP_HEADER_IF_UNMODIFIED_SINCE,
	HTTP_HEADER_MAX_FORWARDS,
	HTTP_HEADER_PROXY_AUTHORIZATION,
	HTTP_HEADER_RANGE,
	HTTP_HEADER_REFERER,
	HTTP_HEADER_TE,
	HTTP_HEADER_USER_AGENT,
	
	// response headers
	HTTP_HEADER_ACCEPT_RANGES,
	HTTP_HEADER_AGE,
	HTTP_HEADER_ETAG,
	HTTP_HEADER_LOCATION,
	HTTP_HEADER_PROXY_AUTHENTICATE,
	HTTP_HEADER_RETRY_AFTER,
	HTTP_HEADER_SERVER,
	HTTP_HEADER_VARY,
	HTTP_HEADER_WWW_AUTHENTICATE,
	
	// entity headers
	HTTP_HEADER_ALLOW,
	HTTP_HEADER_CONTENT_ENCODING,
	HTTP_HEADER_CONTENT_LANGUAGE,
	HTTP_HEADER_CONTENT_LENGTH,
	HTTP_HEADER_CONTENT_LOCATION,
	HTTP_HEADER_CONTENT_MD5,
	HTTP_HEADER_CONTENT_RANGE,
	HTTP_HEADER_CONTENT_TYPE,
	HTTP_HEADER_EXPIRES,
	HTTP_HEADER_LAST_MODIFIED,
} http_header_name_t;

typedef struct {
	list_t              list;
	http_header_name_t  name;
	char               *value;
} http_header_t;

void http_header_init(http_header_t *hs);
void http_header_free(http_header_t *hs);

int  http_header_add(http_header_t *hs, http_header_name_t name, const char *value);
void http_header_del(http_header_t *hs, http_header_name_t name);

#define http_header_for_each(headers, p) \
        list_for_each_entry(p, &(headers->list), list)

typedef enum {
	HTTP_METHOD_UNKNOWN,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_GET,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_CONNECT,
} http_method_t;

typedef struct {
	http_method_t   method;
	char           *path;
	int             major;
	int             minor;
	http_header_t   headers;
} http_request_t;

int http_request_fromstr(const char *str, http_request_t *request);
int http_request_tostr(http_request_t *request, char **str);

typedef enum {
	HTTP_STATUS_UNKNOWN,
	
	// 1xx - Informational
	HTTP_STATUS_CONTINUE,
	HTTP_STATUS_SWITCHING_PROTOCOLS,
	
	// 2xx - Success
	HTTP_STATUS_OK,
	HTTP_STATUS_CREATED,
	HTTP_STATUS_ACCEPTED,
	HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION,
	HTTP_STATUS_NO_CONTENT,
	HTTP_STATUS_RESET_CONTENT,
	HTTP_STATUS_PARTIAL_CONTENT,
	
	// 3xx - Redirection
	HTTP_STATUS_MULTIPLE_CHOICES,
	HTTP_STATUS_MOVED_PERMANENTLY,
	HTTP_STATUS_FOUND,
	HTTP_STATUS_SEE_OTHER,
	HTTP_STATUS_NOT_MODIFIED,
	HTTP_STATUS_USE_PROXY,
	HTTP_STATUS_TEMPORARY_REDIRECT,
	
	// 4xx - client error
	HTTP_STATUS_BAD_REQUEST,
	HTTP_STATUS_UNAUTHORIZED,
	HTTP_STATUS_PAYMENT_REQUIRED,
	HTTP_STATUS_FORBIDDEN,
	HTTP_STATUS_NOT_FOUND,
	HTTP_STATUS_MOTHOD_NOT_ALLOWED,
	HTTP_STATUS_NOT_ACCEPTABLE,
	HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED,
	HTTP_STATUS_REQUEST_TIMEOUT,
	HTTP_STATUS_CONFLICT,
	HTTP_STATUS_GONE,
	HTTP_STATUS_LENGTH_REQUIRED,
	HTTP_STATUS_PRECONDITION_FAILED,
	HTTP_STATUS_REQUEST_ENTITIY_TOO_LARGE,
	HTTP_STATUS_REQUEST_URI_TOO_LARGE,
	HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE,
	HTTP_STATUS_REQUEST_RANGE_NOT_SATISFIABLE,
	HTTP_STATUS_EXPECTATION_FAILED,
	
	// 5xx - server error
	HTTP_STATUS_INTERNAL_SERVER_ERROR,
	HTTP_STATUS_NOT_IMPLEMENTED,
	HTTP_STATUS_BAD_GATEWAY,
	HTTP_STATUS_SERVICE_UNAVAILABLE,
	HTTP_STATUS_GATEWAY_TIMEOUT,
	HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED,
} http_status_t;

typedef struct {
	http_status_t   status;
	char           *reason;
	int             major;
	int             minor;
	http_header_t   headers;
} http_response_t;

int http_response_fromstr(const char *str, http_response_t *response);
int http_response_tostr(http_response_t *response, char **str);

#endif

/*! @} http */
