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

#include "error.h"
#include "mem.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

#include "internal.h"

void rtti_string_init(const rtti_t *type, void *data)
{
	const char *s = CAST(const char *, data);
	s = NULL;
}

void rtti_string_copy(const rtti_t *type, const void *src, void *dst)
{
	const char *buf = type->encode(type, src);
	error_do return;

	type->decode(type, &buf, dst);
	mem_free((char *)buf);
	error_do return;
}

bool rtti_string_equal(const rtti_t *type, const void *a, const void *b)
{
	const char *s1 = CAST(const char *, a);
	const char *s2 = CAST(const char *, b);
	const int asnull  = type->args[0].i;

	if (!asnull) {
		if (!s1) s1 = "";
		if (!s2) s2 = "";
	}

	return str_equal(s1, s2);
}

char *rtti_string_encode(const rtti_t *type, const void *data)
{
	const char *const s = CAST(const char *, data);
	const int asnull  = type->args[0].i;

	if (!s) {
		if (asnull)
			return str_dup("null");
		else
			return str_dup("\"\"");
	}

	char *buf = (char *)(asnull ? "null" : "");
	_lucid_asprintf(&buf, "\"%s\"", s);
	return buf;
}

char *rtti_string_parse(const char **buf)
{
	stralloc_t _rbuf, *rbuf = &_rbuf;
	int escape = 0;
	char c;

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, '"') {
		error_set(EILSEQ, "expected QUOTE near '%.16s'", *buf);
		return NULL;
	}

	stralloc_init(rbuf);

	for (c = **buf; c != '\0'; (*buf)++, c = **buf) {
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
				error_set(EILSEQ, "illegal escape sequence near '%.16s'",
						*buf);
				return NULL;
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

	SKIP_CHAR(buf, '"') {
		stralloc_free(rbuf);
		error_set(EILSEQ,
				"expected QUOTE near '%.16s'", *buf);
		return NULL;
	}

	char *str = stralloc_finalize(rbuf);
	stralloc_free(rbuf);
	return str;
}

void rtti_string_decode(const rtti_t *type, const char **buf, void *data)
{
	const int asnull  = type->args[0].i;

	SKIP_SPACE(buf);

	if (str_cmpn(*buf, "null", 4) == 0) {
		if (!asnull)
			CAST(char *, data) = str_dup("");
		else
			CAST(char *, data) = NULL;
		*buf += 4;
		return;
	}

	char *sbuf = rtti_string_parse(buf);
	error_do return;

	CAST(char *, data) = sbuf;
}

void rtti_string_free(const rtti_t *type, void *data)
{
	char *str = CAST(char *, data);

	if (str) {
		mem_free(str);
		CAST(char *, data) = NULL;
	}
}
