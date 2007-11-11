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

char *rtti_inaddr_encode(const rtti_t *type, const void *data)
{
	struct in_addr inaddr = CAST(struct in_addr, data);
	char *buf = NULL;
	asprintf(&buf, "\"%s\"", inet_ntoa(inaddr));
	return buf;
}

void rtti_inaddr_decode(const rtti_t *type, const char **buf, void *data)
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
