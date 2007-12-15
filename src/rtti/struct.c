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
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

#include "internal.h"

#define MEMBP(F, D) ((void *)((char *)(D) + (F)->offset))

void rtti_struct_init(const rtti_t *type, void *data)
{
	CHECK_TYPE(STRUCT);

	mem_set(data, 0, type->size);

	const rtti_field_t *field;
	for (field = type->args[0].v; field->name != NULL; field++) {
		void *memb = MEMBP(field, data);
		field->type->init(field->type, memb);
		error_dof("failed to initialize struct (%s)", field->name)
			return;
	}
}

void rtti_struct_copy(const rtti_t *type, const void *src, void *dst)
{
	CHECK_TYPE(STRUCT);

	mem_set(dst, 0, type->size);

	const rtti_field_t *field;
	for (field = type->args[0].v; field->name != NULL; field++) {
		const void *smemb = MEMBP(field, src);
		void *dmemb = MEMBP(field, dst);

		field->type->copy(field->type, smemb, dmemb);
		error_dof("failed to copy struct (%s)", field->name)
			return;
	}
}

bool rtti_struct_equal(const rtti_t *type, const void *a, const void *b)
{
	CHECK_TYPE(STRUCT);

	const rtti_field_t *field;
	for (field = type->args[0].v; field->name != NULL; field++) {
		const void *amemb = MEMBP(field, a);
		const void *bmemb = MEMBP(field, b);

		bool eq = field->type->equal(field->type, amemb, bmemb);
		error_dof("failed to compare struct (%s)", field->name)
			return false;

		if (!eq)
			return false;
	}

	return true;
}

char *rtti_struct_encode(const rtti_t *type, const void *data)
{
	CHECK_TYPE(STRUCT);

	stralloc_t _buf, *buf = &_buf;
	stralloc_init(buf);
	stralloc_cats(buf, "{");

	const rtti_field_t *field;
	for (field = type->args[0].v; field->name != NULL; field++) {
		const rtti_t *ktype = &rtti_string_type;
		void *memb = MEMBP(field, data);

		char *kbuf = ktype->encode(ktype, &field->name);
		error_dof("failed to encode struct (%s)", field->name)
			return NULL;

		char *vbuf = field->type->encode(field->type, memb);
		error_dof("failed to encode struct (%s)", field->name)
			return NULL;

		stralloc_catm(buf, kbuf, ":", vbuf, NULL);

		if ((field + 1)->name != NULL)
			stralloc_cats(buf, ",");
	}

	stralloc_cats(buf, "}");
	return stralloc_finalize(buf);
}

void rtti_struct_decode(const rtti_t *type, const char **buf, void *data)
{
	CHECK_TYPE(STRUCT);

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, '{') {
		error_set(EILSEQ, "expected LCUBR near '%.16s'", *buf);
		return;
	}

	while (1) {
		const rtti_t *ktype = &rtti_string_type;
		const char *kbuf;
		ktype->decode(ktype, buf, &kbuf);
		error_dof("failed to decode struct key")
			return;

		const rtti_field_t *field;
		for (field = type->args[0].v; field->name != NULL; field++) {
			if (str_equal(field->name, kbuf))
				break;
		}

		if (field->name == NULL) {
			error_set(ENOENT, "unknown struct member (%s)", kbuf);
			return;
		}

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, ':') {
			error_set(EILSEQ, "expected COLON near '%.16s'", *buf);
			return;
		}

		void *memb = MEMBP(field, data);
		field->type->decode(field->type, buf, memb);
		error_dof("failed to decode struct member (%s)", kbuf)
			return;

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, ',') {
			break;
		}
	}

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, '}') {
		error_set(EILSEQ, "expected RCUBR near '%.16s'", *buf);
		return;
	}
}

size_t rtti_struct_length(const rtti_t *type)
{
	const rtti_field_t *field = type->args[0].v;
	size_t length = 0;

	while (field->name != NULL) {
		length++;
		field++;
	}

	return length;
}
