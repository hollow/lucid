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
#include <stdint.h>
#include <string.h>

#include "base64.h"
#include "log.h"

static
int base64_encode_t(void)
{
	int i, rc = 0;
	char *base64 = NULL;

	struct test {
		const char *str;
		const char *base64;
	} T[] = {
		{ "Hello World!", "SGVsbG8gV29ybGQh" },
		{ "a", "YQ==" },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		base64 = base64_encode(T[i].str, strlen(T[i].str));

		if (!base64 || strcmp(base64, T[i].base64))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].base64, base64);

		if (base64)
			free(base64);
	}

	return rc;
}

static
int base64_decode_t(void)
{
	int i, rc = 0;
	char *str = NULL;
	size_t len = 0;

	struct test {
		const char *str;
		const char *base64;
	} T[] = {
		{ "Hello World!", "SGVsbG8gV29ybGQh" },
		{ "a", "YQ==" },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		str = base64_decode(T[i].base64, &len);

		if (!str || strcmp(str, T[i].str) || strlen(T[i].base64) != len)
			rc += log_error("[%s/%02d] E[%s,%d] R[%s,%d]",
			                __FUNCTION__, i,
			                T[i].str, strlen(T[i].base64),
			                str, len);

		if (str)
			free(str);
	}

	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;

	log_options_t log_options = {
		.log_ident  = "base64",
		.log_dest  = LOGD_STDERR,
		.log_opts  = LOGO_PRIO|LOGO_IDENT,
	};

	log_init(&log_options);

	rc += base64_encode_t();
	rc += base64_decode_t();

	log_close();

	return rc;
}
