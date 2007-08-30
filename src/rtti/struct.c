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
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

#include "internal.h"

#define IDX(D, F) ((char *)(D) + (F)->offset)

int rtti_struct_init(const rtti_t *type, void *data)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	mem_set(data, 0, type->size);
	for (field = type->args[0].v; field->name != NULL; field++) {
		if (field->type->init(field->type, IDX(data, field)) == -1)
			break;
	}

	if (field->name != NULL) {
		while (field-- != type->args[0].v)
			field->type->uninit(field->type, IDX(data, field));
		mem_set(data, 0, type->size);
		return -1;
	}

	return 0;
}

int rtti_struct_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	mem_set(dst, 0, type->size);
	for (field = type->args[0].v; field->name != NULL; field++) {
		const void *const sdata = IDX(src, field);
		void *const ddata = IDX(dst, field);

		if (field->type->copy(field->type, sdata, ddata) == -1)
			break;
	}

	if (field->name != NULL) {
		while (field-- != type->args[0].v)
			field->type->uninit(field->type, IDX(dst, field));
		mem_set(dst, 0, type->size);
		return -1;
	}

	return 0;
}

int rtti_struct_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	for (field = type->args[0].v; field->name != NULL; field++) {
		const void *const data1 = IDX(a, field);
		const void *const data2 = IDX(b, field);

		if (!field->type->equal(field->type, data1, data2))
			return 0;
	}

	return 1;
}

static
const rtti_t rtti_struct_key_type = RTTI_STRING_TYPE(0, 0);

int rtti_struct_encode(const rtti_t *type, const void *data, char **buf)
{
	const rtti_t *ftype, *ktype = &rtti_struct_key_type;
	const rtti_field_t *field;
	stralloc_t _rbuf, *rbuf = &_rbuf;
	char *fbuf;

	CHECK_TYPE(STRUCT);

	*buf = NULL;
	stralloc_init(rbuf);
	stralloc_cats(rbuf, "{");

	for (field = type->args[0].v; field->name != NULL; field++) {
		ftype = field->type;

		if (ktype->encode(ktype, &field->name, &fbuf) == -1)
			goto out;

		stralloc_catm(rbuf, fbuf, ":", NULL);
		mem_free(fbuf);

		if (ftype->encode(ftype, IDX(data, field), &fbuf) == -1)
			goto out;

		stralloc_cats(rbuf, fbuf);
		mem_free(fbuf);

		if ((field + 1)->name != NULL)
			stralloc_cats(rbuf, ",");
	}

	stralloc_cats(rbuf, "}");
	*buf = stralloc_finalize(rbuf);

out:
	stralloc_free(rbuf);
	return *buf ? str_len(*buf) : -1;
}

int rtti_struct_decode(const rtti_t *type, const char *buf, void *data)
{
	const rtti_t *ftype, *ktype = &rtti_struct_key_type;
	const char *key, *p = buf;
	void *fdata;
	int res;

	CHECK_TYPE(STRUCT);

	if (type->init(type, data) == -1)
		return -1;

	SKIP_SPACE(p);
	SKIP_CHAR(p, '{') {
		PARSE_ERROR(EINVAL, buf, p);
	}

	while (1) {
		errno = 0;
		switch ((res = ktype->decode(ktype, p, &key))) {
		case  0: PARSE_ERROR(EINVAL, buf, p);
		case -1: return -1;
		default: p += res;
		}

		if (errno)
			PARSE_ERROR(errno, buf, p);

		fdata = data;

		if ((ftype = rtti_find(type, key, &fdata)) == NULL)
			PARSE_ERROR(ENOENT, buf, p);

		SKIP_SPACE(p);
		SKIP_CHAR(p, ':') {
			PARSE_ERROR(EINVAL, buf, p);
		}

		errno = 0;
		switch ((res = ftype->decode(ftype, p, fdata))) {
		case  0: PARSE_ERROR(EINVAL, buf, p);
		case -1: return -1;
		default: p += res;
		}

		if (errno)
			PARSE_ERROR(errno, buf, p);

		SKIP_SPACE(p);
		SKIP_CHAR(p, ',') {
			break;
		}
	}

	SKIP_SPACE(p);
	SKIP_CHAR(p, '}') {
		PARSE_ERROR(EINVAL, buf, p);
	}

	PARSE_OK(buf, p);
}

int rtti_struct_free(const rtti_t *type, void *data)
{
	const rtti_field_t *field;

	CHECK_TYPE(STRUCT);

	for (field = type->args[0].v; field->name != NULL; field++)
		field->type->uninit(field->type, IDX(data, field));
	mem_set(data, 0, type->size);

	return 0;
}
