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
#include <string.h>

#include "error.h"
#include "char.h"
#include "cext.h"
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

void rtti_init(const rtti_t *type, void *data)
{
	type->init(type, data);
	error_do return;
}

void rtti_copy(const rtti_t *type, const void *src, void *dst)
{
	type->copy(type, src, dst);
	error_do return;
}

bool rtti_equal(const rtti_t *type, const void *data1, const void *data2)
{
	bool eq = type->equal(type, data1, data2);
	error_do return false;
	return eq;
}

char *rtti_encode(const rtti_t *type, const void *data)
{
	char *buf = type->encode(type, data);
	error_do return NULL;
	return buf;
}

void rtti_decode(const rtti_t *type, const char **buf, void *data)
{
	type->decode(type, buf, data);
	error_do return;
}

size_t rtti_length(const rtti_t *type, const void *data)
{
	switch (type->tclass) {
	case RTTI_TYPE_STRUCT: return rtti_struct_length(type);
	case RTTI_TYPE_LIST:   return rtti_list_length(type, data);
	default: return 0;
	}
}

void rtti_region_init(const rtti_t *type, void *data)
{
	memset(data, 0, type->size);
}

void rtti_region_copy(const rtti_t *type, const void *src, void *dst)
{
	memcpy(dst, src, type->size);
}

bool rtti_region_equal(const rtti_t *type, const void *a, const void *b)
{
	return memcmp(a, b, type->size) == 0;
}

void rtti_notsup_init(const rtti_t *type, void *data)
{
	error_set(ENOTSUP, "rtti_init not supported for type (%s)",
			type->name);
}

void rtti_notsup_copy(const rtti_t *type, const void *src, void *dst)
{
	error_set(ENOTSUP, "rtti_copy not supported for type (%s)",
			type->name);
}

bool rtti_notsup_equal(const rtti_t *type, const void *a, const void *b)
{
	error_set(ENOTSUP, "rtti_equal not supported for type (%s)",
			type->name);
	return false;
}

char *rtti_notsup_encode(const rtti_t *type, const void *data)
{
	error_set(ENOTSUP, "rtti_encode not supported for type (%s)",
			type->name);
	return NULL;
}

void rtti_notsup_decode(const rtti_t *type, const char **buf, void *data)
{
	error_set(ENOTSUP, "rtti_decode not supported for type (%s)",
			type->name);
}

void rtti_get_parser_offset(const char *orig, const char *parsed,
		int *_line, int *_column)
{
	int line = 1, column = 1;
	const char *origp = orig;

	while (origp < parsed) {
		switch (*origp) {
		case '\n':
			line++;
			column = 1;
			break;

		case '\t':
			column += 4;
			break;

		default:
			column++;
			break;
		}

		origp++;
	}

	*_line = line;
	*_column = column;
}

#define ADD_INDENTION(sa) do { \
	int s = indent; \
	while (s--) stralloc_cats(sa, "\t"); \
} while (0)

char *rtti_beautify(const char *str)
{
	int i, indent = 0, n = str_len(str);
	bool escape = false, instring = false;
	stralloc_t _sa, *sa = &_sa;

	stralloc_init(sa);

	for (i = 0; i < n; i++) {
		if (instring) {
			if (escape) {
				stralloc_catb(sa, &str[i], 1);
				escape = false;
			}

			else if (str[i] == '\\') {
				stralloc_cats(sa, "\\");
				escape = true;
			}

			else if (str[i] == '"') {
				stralloc_cats(sa, "\"");
				instring = false;
			}

			else {
				stralloc_catb(sa, &str[i], 1);
			}

			continue;
		}

		switch (str[i]) {
		case '{':
		case '[':
			stralloc_catb(sa, &str[i], 1);
			stralloc_cats(sa, "\n");
			indent++;
			ADD_INDENTION(sa);
			break;

		case '}':
		case ']':
			stralloc_cats(sa, "\n");
			indent--;
			ADD_INDENTION(sa);
			stralloc_catb(sa, &str[i], 1);
			break;

		case ':':
			stralloc_cats(sa, " : ");
			break;

		case ',':
			stralloc_cats(sa, ",\n");
			ADD_INDENTION(sa);
			break;

		case '"':
			instring = true;
			stralloc_cats(sa, "\"");
			break;

		default:
			stralloc_catb(sa, &str[i], 1);
		}
	}

	return stralloc_finalize(sa);
}
