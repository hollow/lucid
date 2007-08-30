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
#include <errno.h>

#include "mem.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

#include "internal.h"

int rtti_string_init(const rtti_t *type, void *data)
{
	return rtti_string_decode(type, "\"\"", data) ? 0 : -1;
}

int rtti_string_copy(const rtti_t *type, const void *src, void *dst)
{
	char *buf;

	if (type->encode(type, src, &buf) == -1)
		return -1;

	int res = type->decode(type, buf, dst);
	mem_free(buf);
	return res;
}

int rtti_string_equal(const rtti_t *type, const void *a, const void *b)
{
	const char *const s1 = CAST(const char *, a);
	const char *const s2 = CAST(const char *, b);
	const int maynull = type->args[0].i;
	const int asnull  = type->args[1].i;
	int n1, n2;

	if (!maynull && (!s1 || !s2))
		return errno = EINVAL, 0;

	if (asnull)
		return str_equal(s1, s2);

	n1 = (!s1 || !*s1);
	n2 = (!s2 || !*s2);

	if (n1 ^ n2)
		return 0;
	if (n1)
		return 1;

	return str_equal(s1, s2);
}

int rtti_string_encode(const rtti_t *type, const void *data, char **buf)
{
	const char *const s = CAST(const char *, data);
	const int maynull = type->args[0].i;
	const int asnull  = type->args[1].i;

	if (!maynull && !s)
		return errno = EINVAL, 0;

	if (!s) {
		if (asnull)
			return _lucid_asprintf(buf, "null");
		else
			return _lucid_asprintf(buf, "\"\"");
	}

	return _lucid_asprintf(buf, "\"%s\"", s);
}

int rtti_string_parse(const char *str, char **buf)
{
	stralloc_t _rbuf, *rbuf = &_rbuf;
	const char *p = str;
	int escape;
	char c;

	SKIP_CHAR(p, '"') {
		PARSE_ERROR(EINVAL, str, p);
	}

	stralloc_init(rbuf);

	for (c = *p; c != '\0'; p++, c = *p) {
		if (escape) {
			switch (c) {
			case 'b': c = '\b'; break;
			case 'f': c = '\f'; break;
			case 'n': c = '\n'; break;
			case 'r': c = '\r'; break;
			case 't': c = '\t'; break;
			case '"':
			case '/':
			case '\\': break;
			default:
				stralloc_free(rbuf);
				PARSE_ERROR(EINVAL, str, p);
			}

			stralloc_catb(rbuf, &c, 1);
			escape = 0;
		}

		else if (c == '\\')
			escape = 1;

		else if (c == '"')
			break;

		else
			stralloc_catb(rbuf, &c, 1);
	}

	SKIP_CHAR(p, '"') {
		stralloc_free(rbuf);
		PARSE_ERROR(EINVAL, str, p);
	}

	*buf = stralloc_finalize(rbuf);
	stralloc_free(rbuf);
	PARSE_OK(str, p);
}

int rtti_string_decode(const rtti_t *type, const char *buf, void *data)
{
	const int maynull = type->args[0].i;
	const int asnull  = type->args[1].i;
	const char *p = buf;
	char *sbuf;
	int res;

	SKIP_SPACE(p);

	if (str_cmpn(p, "null", 4) == 0) {
		if (!maynull)
			PARSE_ERROR(EINVAL, buf, p);
		else if (!asnull)
			CAST(char *, data) = str_dup("");
		else
			CAST(char *, data) = NULL;
		PARSE_OK(buf, p + 4);
	}

	errno = 0;
	if ((res = rtti_string_parse(p, &sbuf)) == -1)
		return -1;
	else
		p += res;

	if (errno)
		PARSE_ERROR(errno, buf, p);

	CAST(char *, data) = sbuf;
	PARSE_OK(buf, p);
}

int rtti_string_free(const rtti_t *type, void *data)
{
	char *const s = CAST(char *, data);

	if (s) {
		mem_free(s);
		CAST(char *, data) = NULL;
	}

	return 0;
}
