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

#define IDX(D, F) ((char *)(D) + (F)->offset)

void rtti_struct_init(const rtti_t *type, void *data)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	mem_set(data, 0, type->size);

	for (field = type->args[0].v; field->name != NULL; field++) {
		field->type->init(field->type, IDX(data, field));
		error_dof("failed to initialize struct (%s)", field->name) {
			while (field-- != type->args[0].v)
				field->type->uninit(field->type, IDX(data, field));
			mem_set(data, 0, type->size);
			return;
		}
	}
}

void rtti_struct_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	mem_set(dst, 0, type->size);

	for (field = type->args[0].v; field->name != NULL; field++) {
		const void *const sdata = IDX(src, field);
		void *const ddata = IDX(dst, field);

		field->type->copy(field->type, sdata, ddata);
		error_dof("failed to copy array (%s)", field->name) {
			while (field-- != type->args[0].v)
				field->type->uninit(field->type, IDX(dst, field));
			mem_set(dst, 0, type->size);
			return;
		}
	}
}

bool rtti_struct_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	for (field = type->args[0].v; field->name != NULL; field++) {
		const void *const data1 = IDX(a, field);
		const void *const data2 = IDX(b, field);

		bool eq = field->type->equal(field->type, data1, data2);
		error_dof("failed to compare struct (%s)", field->name)
			return false;

		if (!eq)
			return false;
	}

	return true;
}

char *rtti_struct_encode(const rtti_t *type, const void *data)
{
	const rtti_t *ktype = &rtti_string_type;
	const rtti_field_t *field;
	stralloc_t _rbuf, *rbuf = &_rbuf;
	char *fbuf, *buf = NULL;

	CHECK_TYPE(STRUCT);

	stralloc_init(rbuf);
	stralloc_cats(rbuf, "{");

	for (field = type->args[0].v; field->name != NULL; field++) {
		fbuf = ktype->encode(ktype, &field->name);
		error_dof("failed to encode struct (%s)", field->name)
			goto out;

		stralloc_catm(rbuf, fbuf, ":", NULL);
		mem_free(fbuf);

		fbuf = field->type->encode(field->type, IDX(data, field));
		error_dof("failed to encode struct (%s)", field->name)
			goto out;

		stralloc_cats(rbuf, fbuf);
		mem_free(fbuf);

		if ((field + 1)->name != NULL)
			stralloc_cats(rbuf, ",");
	}

	stralloc_cats(rbuf, "}");
	buf = stralloc_finalize(rbuf);

out:
	stralloc_free(rbuf);
	return buf;
}

void rtti_struct_decode(const rtti_t *type, const char **buf, void *data)
{
	const rtti_t *ftype, *ktype = &rtti_string_type;
	const char *key;
	void *fdata;

	CHECK_TYPE(STRUCT);

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, '{') {
		error_set(EILSEQ, "expected LCUBR near '%.16s'", *buf);
		return;
	}

	while (1) {
		ktype->decode(ktype, buf, &key);
		error_dof("failed to decode struct near '%.16s'", *buf)
			return;

		fdata = data;

		ftype = rtti_find(type, key, &fdata);
		ktype->uninit(ktype, &key);
		error_dof("list member does not exist (%s)", key) {
			return;
		}

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, ':') {
			error_set(EILSEQ, "expected COLON near '%.16s'", *buf);
			return;
		}

		ftype->decode(ftype, buf, fdata);
		error_dof("failed to decode struct (%s) near '%.16s'", ftype->name, *buf)
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

void rtti_struct_free(const rtti_t *type, void *data)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	for (field = type->args[0].v; field->name != NULL; field++)
		field->type->uninit(field->type, IDX(data, field));
	mem_set(data, 0, type->size);
}
