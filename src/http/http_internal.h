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

#ifndef _LUCID_HTTP_INTERNAL_H
#define _LUCID_HTTP_INTERNAL_H

#include "str.h"

struct headername_map {
	http_header_name_t name;
	char *str;
};

static struct headername_map HEADERNAME_MAP[] = {
	{ HTTP_HEADER_CACHE_CONTROL,       "Cache-Control" },
	{ HTTP_HEADER_CONNECTION,          "Connection" },
	{ HTTP_HEADER_DATE,                "Date" },
	{ HTTP_HEADER_PRAGMA,              "Pragma" },
	{ HTTP_HEADER_TRAILER,             "Trailer" },
	{ HTTP_HEADER_TRANFSER_ENCODING,   "Transfer-Encoding" },
	{ HTTP_HEADER_UPGRADE,             "Upgrade" },
	{ HTTP_HEADER_VIA,                 "Via" },
	{ HTTP_HEADER_WARNING,             "Warning" },
	{ HTTP_HEADER_ACCEPT,              "Accept" },
	{ HTTP_HEADER_ACCEPT_CHARSET,      "Accept-Charset" },
	{ HTTP_HEADER_ACCEPT_ENCODING,     "Accept-Encoding" },
	{ HTTP_HEADER_ACCEPT_LANGUAGE,     "Accept-Language" },
	{ HTTP_HEADER_AUTHORIZATION,       "Authorization" },
	{ HTTP_HEADER_EXPECT,              "Expect" },
	{ HTTP_HEADER_FROM,                "From" },
	{ HTTP_HEADER_HOST,                "Host" },
	{ HTTP_HEADER_IF_MATCH,            "If-Match" },
	{ HTTP_HEADER_IF_MODIFIED_SINCE,   "If-Modified-Since" },
	{ HTTP_HEADER_IF_NONE_MATCH,       "If-None-Match" },
	{ HTTP_HEADER_IF_RANGE,            "If-Range" },
	{ HTTP_HEADER_IF_UNMODIFIED_SINCE, "If-Unmodified-Since" },
	{ HTTP_HEADER_MAX_FORWARDS,        "Max-Forwards" },
	{ HTTP_HEADER_PROXY_AUTHORIZATION, "Proxy-Authorization" },
	{ HTTP_HEADER_RANGE,               "Range" },
	{ HTTP_HEADER_REFERER,             "Referer" },
	{ HTTP_HEADER_TE,                  "TE" },
	{ HTTP_HEADER_USER_AGENT,          "User-Agent" },
	{ HTTP_HEADER_ACCEPT_RANGES,       "Accept-Ranges" },
	{ HTTP_HEADER_AGE,                 "Age" },
	{ HTTP_HEADER_ETAG,                "ETag" },
	{ HTTP_HEADER_LOCATION,            "Location" },
	{ HTTP_HEADER_PROXY_AUTHENTICATE,  "Proxy-Authenticate" },
	{ HTTP_HEADER_RETRY_AFTER,         "Retry-After" },
	{ HTTP_HEADER_SERVER,              "Server" },
	{ HTTP_HEADER_VARY,                "Vary" },
	{ HTTP_HEADER_WWW_AUTHENTICATE,    "WWW-Authenticate" },
	{ HTTP_HEADER_ALLOW,               "Allow" },
	{ HTTP_HEADER_CONTENT_ENCODING,    "Content-Encoding" },
	{ HTTP_HEADER_CONTENT_LANGUAGE,    "Content-Language" },
	{ HTTP_HEADER_CONTENT_LENGTH,      "Content-Length" },
	{ HTTP_HEADER_CONTENT_LOCATION,    "Content-Location" },
	{ HTTP_HEADER_CONTENT_MD5,         "Content-MD5" },
	{ HTTP_HEADER_CONTENT_RANGE,       "Content-Range" },
	{ HTTP_HEADER_CONTENT_TYPE,        "Content-Type" },
	{ HTTP_HEADER_EXPIRES,             "Expires" },
	{ HTTP_HEADER_LAST_MODIFIED,       "Last-Modified" },
};

struct method_map {
	http_method_t name;
	char *str;
};

static struct method_map METHOD_MAP[] = {
	{ HTTP_METHOD_OPTIONS, "OPTIONS" },
	{ HTTP_METHOD_GET,     "GET" },
	{ HTTP_METHOD_HEAD,    "HEAD" },
	{ HTTP_METHOD_POST,    "POST" },
	{ HTTP_METHOD_PUT,     "PUT" },
	{ HTTP_METHOD_DELETE,  "DELETE" },
	{ HTTP_METHOD_TRACE,   "TRACE" },
	{ HTTP_METHOD_CONNECT, "CONNECT" },
};

struct status_map {
	http_status_t name;
	char *str;
};

static struct status_map STATUS_MAP[] = {
	{ HTTP_STATUS_CONTINUE,                      "100" },
	{ HTTP_STATUS_SWITCHING_PROTOCOLS,           "101" },
	{ HTTP_STATUS_OK,                            "200" },
	{ HTTP_STATUS_CREATED,                       "201" },
	{ HTTP_STATUS_ACCEPTED,                      "202" },
	{ HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION, "203" },
	{ HTTP_STATUS_NO_CONTENT,                    "204" },
	{ HTTP_STATUS_RESET_CONTENT,                 "205" },
	{ HTTP_STATUS_PARTIAL_CONTENT,               "206" },
	{ HTTP_STATUS_MULTIPLE_CHOICES,              "300" },
	{ HTTP_STATUS_MOVED_PERMANENTLY,             "301" },
	{ HTTP_STATUS_FOUND,                         "302" },
	{ HTTP_STATUS_SEE_OTHER,                     "303" },
	{ HTTP_STATUS_NOT_MODIFIED,                  "304" },
	{ HTTP_STATUS_USE_PROXY,                     "305" },
	{ HTTP_STATUS_TEMPORARY_REDIRECT,            "307" },
	{ HTTP_STATUS_BAD_REQUEST,                   "400" },
	{ HTTP_STATUS_UNAUTHORIZED,                  "401" },
	{ HTTP_STATUS_PAYMENT_REQUIRED,              "402" },
	{ HTTP_STATUS_FORBIDDEN,                     "403" },
	{ HTTP_STATUS_NOT_FOUND,                     "404" },
	{ HTTP_STATUS_MOTHOD_NOT_ALLOWED,            "405" },
	{ HTTP_STATUS_NOT_ACCEPTABLE,                "406" },
	{ HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED, "407" },
	{ HTTP_STATUS_REQUEST_TIMEOUT,               "408" },
	{ HTTP_STATUS_CONFLICT,                      "409" },
	{ HTTP_STATUS_GONE,                          "410" },
	{ HTTP_STATUS_LENGTH_REQUIRED,               "411" },
	{ HTTP_STATUS_PRECONDITION_FAILED,           "412" },
	{ HTTP_STATUS_REQUEST_ENTITIY_TOO_LARGE,     "413" },
	{ HTTP_STATUS_REQUEST_URI_TOO_LARGE,         "414" },
	{ HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE,        "415" },
	{ HTTP_STATUS_REQUEST_RANGE_NOT_SATISFIABLE, "416" },
	{ HTTP_STATUS_EXPECTATION_FAILED,            "417" },
	{ HTTP_STATUS_INTERNAL_SERVER_ERROR,         "500" },
	{ HTTP_STATUS_NOT_IMPLEMENTED,               "501" },
	{ HTTP_STATUS_BAD_GATEWAY,                   "502" },
	{ HTTP_STATUS_SERVICE_UNAVAILABLE,           "503" },
	{ HTTP_STATUS_GATEWAY_TIMEOUT,               "504" },
	{ HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED,    "505" },
};

static inline
http_header_name_t str_to_headername(const char *str)
{
	int i;
	
	for (i = 0; i < sizeof(HEADERNAME_MAP)/sizeof(HEADERNAME_MAP[0]); i++)
		if (str_equal(HEADERNAME_MAP[i].str, str))
			return HEADERNAME_MAP[i].name;
	
	return HTTP_HEADER_UNKNOWN;
}

static inline
const char *headername_to_str(http_header_name_t name)
{
	int i;
	
	for (i = 0; i < sizeof(HEADERNAME_MAP)/sizeof(HEADERNAME_MAP[0]); i++)
		if (HEADERNAME_MAP[i].name == name)
			return HEADERNAME_MAP[i].str;
	
	return errno = EINVAL, NULL;
}

static inline
http_method_t str_to_method(const char *str)
{
	int i;
	
	for (i = 0; i < sizeof(METHOD_MAP)/sizeof(METHOD_MAP[0]); i++)
		if (str_equal(METHOD_MAP[i].str, str))
			return METHOD_MAP[i].name;
	
	return HTTP_METHOD_UNKNOWN;
}

static inline
const char *method_to_str(http_method_t name)
{
	int i;
	
	for (i = 0; i < sizeof(METHOD_MAP)/sizeof(METHOD_MAP[0]); i++)
		if (METHOD_MAP[i].name == name)
			return METHOD_MAP[i].str;
	
	return errno = EINVAL, NULL;
}

static inline
http_status_t str_to_status(const char *str)
{
	int i;
	
	for (i = 0; i < sizeof(STATUS_MAP)/sizeof(STATUS_MAP[0]); i++)
		if (str_equal(STATUS_MAP[i].str, str))
			return STATUS_MAP[i].name;
	
	return HTTP_STATUS_UNKNOWN;
}

static inline
const char *status_to_str(http_status_t name)
{
	int i;
	
	for (i = 0; i < sizeof(STATUS_MAP)/sizeof(STATUS_MAP[0]); i++)
		if (STATUS_MAP[i].name == name)
			return STATUS_MAP[i].str;
	
	return errno = EINVAL, NULL;
}

#endif
