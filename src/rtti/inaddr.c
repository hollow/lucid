// Copyright (c) 2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "error.h"
#include "mem.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

char *rtti_in_addr_encode(const rtti_t *type, const void *data)
{
	struct in_addr inaddr = CAST(struct in_addr, data);
	char *buf = NULL;
	asprintf(&buf, "\"%s\"", inet_ntoa(inaddr));
	return buf;
}

void rtti_in_addr_decode(const rtti_t *type, const char **buf, void *data)
{
	struct in_addr *inaddr = &(CAST(struct in_addr, data));

	SKIP_SPACE(buf);

	if (str_cmpn(*buf, "null", 4) == 0) {
		inaddr->s_addr = INADDR_ANY;
		return;
	}

	char *sbuf = rtti_string_parse(buf);
	error_do return;

	if (inet_aton(sbuf, inaddr) < 0) {
		error_set(errno, "failed to convert ip address");
	}

	mem_free(sbuf);
}

char *rtti_port_encode(const rtti_t *type, const void *data)
{
	uint16_t port = CAST(uint16_t, data);
	port = ntohs(port);
	return rtti_int_encode(type, &port);
}

void rtti_port_decode(const rtti_t *type, const char **buf, void *data)
{
	uint16_t *port = &(CAST(uint16_t, data));
	rtti_int_decode(type, buf, port);
	*port = htons(*port);
}

char *rtti_sockaddr_in_encode(const rtti_t *type, const void *data)
{
	struct sockaddr_in addr = CAST(struct sockaddr_in, data);
	char *buf = NULL;
	asprintf(&buf, "\"%s:%d\"", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	return buf;
}

void rtti_sockaddr_in_decode(const rtti_t *type, const char **buf, void *data)
{
	struct sockaddr_in *inaddr = &(CAST(struct sockaddr_in, data));

	SKIP_SPACE(buf);

	if (str_cmpn(*buf, "null", 4) == 0) {
		inaddr->sin_addr.s_addr = INADDR_ANY;
		inaddr->sin_port = 0;
		*buf += 4;
		return;
	}

	char *ip = rtti_string_parse(buf);
	error_do return;

	char *port = str_rchr(ip, ':', str_len(ip));
	if (port)
		*port++ = '\0';

	if (inet_aton(ip, &(inaddr->sin_addr)) < 0) {
		error_set(errno, "failed to convert ip address");
	}

	rtti_decode(&rtti_uint16_type, NULL, (const char **)&port, &(inaddr->sin_port));
	inaddr->sin_port = htons(inaddr->sin_port);

	mem_free(ip);
}
