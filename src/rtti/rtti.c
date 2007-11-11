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
#include "mem.h"
#include "rtti.h"
#include "str.h"

/* array length is a read-only field */
static const rtti_t rtti_type_array_length = {
	sizeof(size_t),
	"size_t",
	RTTI_TYPE_PRIMITIVE,
	rtti_region_init,
	rtti_region_copy,
	rtti_region_equal,
	rtti_int_encode,
	rtti_notsup_decode,
	rtti_nothing_free,
	{ { (void *)2 }, { (void *)0 } }    /* args for structs_int_ascify */
};

void rtti_init(const rtti_t *type, const char *name, void *data)
{
	/* find item if any */
	type = rtti_find(type, name, &data);
	error_do return;

	/* initialize data */
	type->init(type, data);
	error_do return;
}

void rtti_free(const rtti_t *type, const char *name, void *data)
{
	/* find item if any */
	type = rtti_find(type, name, &data);
	error_do return;

	/* uninitialize data */
	type->uninit(type, data);
	error_do return;
}

void rtti_get(const rtti_t *type, const char *name,
		const void *src, void *dst)
{
	/* find item if any */
	type = rtti_find(type, name, (void **)&src);
	error_do return;

	/* get a copy of the data */
	type->copy(type, src, dst);
	error_do return;
}

void rtti_set(const rtti_t *type, const char *name,
		const void *src, void *dst)
{
	/* find item if any */
	type = rtti_find(type, name, &dst);
	error_do return;

	/* make a copy of src */
	void *copy = mem_alloc(type->size);

	type->copy(type, src, copy);
	error_do {
		mem_free(copy);
		return;
	}

	/* free dst item */
	type->uninit(type, dst);
	error_do {
		mem_free(copy);
		return;
	}

	/* set copy in dst */
	mem_cpy(dst, copy, type->size);
	mem_free(copy);
}

char *rtti_encode(const rtti_t *type, const char *name, const void *data)
{
	/* find item if any */
	type = rtti_find(type, name, (void **)&data);
	error_do return NULL;

	/* encode item */
	char *buf = type->encode(type, data);
	error_do return NULL;

	return buf;
}

void rtti_decode(const rtti_t *type, const char *name,
		const char **buf, void *data)
{
	/* find item if any */
	type = rtti_find(type, name, &data);
	error_do return;

	/* decode item */
	type->decode(type, buf, data);
	error_do return;
}

bool rtti_equal(const rtti_t *type, const char *name,
		const void *data1, const void *data2)
{
	/* find items if any */
	rtti_find(type, name, (void **)&data1);
	error_do return false;

	type = rtti_find(type, name, (void **)&data2);
	error_do return false;

	/* compare items */
	int eq = type->equal(type, data1, data2);
	error_do return false;

	return eq;
}

const rtti_t *rtti_find(const rtti_t *type, const char *name, void **datap)
{
	void *data = *datap;
	const char *next;

	/* stop recursing on empty string */
	if (name == NULL || *name == '\0')
		return type;

	/* primitive types don't have sub-elements */
	if (type->tclass == RTTI_TYPE_PRIMITIVE) {
		error_set(ENOENT, "no sub-elements in primitive type (%s)",
				type->name);
		return NULL;
	}

	/* dereference through pointer(s) */
	while (type->tclass == RTTI_TYPE_POINTER) {
		type = type->args[0].v;
		data = *((void **)data);
	}

	/* get next name component */
	if ((next = str_chr(name, RTTI_SEPARATOR, str_len(name))) != NULL)
		next++;

	/* find element of aggregate structure */
	switch (type->tclass) {
	case RTTI_TYPE_ARRAY: {
		const rtti_t *const etype = type->args[0].v;
		const unsigned int length = type->args[2].i;
		unsigned long long index;
		char *eptr;

		/* special handling for "length" */
		if (str_equal(name, "length")) {
			type = &rtti_type_array_length;
			data = (void *)&type->args[2].i;
			break;
		}

		/* decode index */
		eptr += str_toumax(name, &index, 0, str_len(name));
		if (!char_isdigit(*name) || eptr <= name ||
				(*eptr != '\0' && *eptr != RTTI_SEPARATOR)) {
			error_set(EILSEQ, "invalid array index syntax (%s)", name);
			return NULL;
		}

		if (index >= length) {
			error_set(ERANGE, "array index out of range (%llu, %u)", index, length);
			return NULL;
		}

		type = etype;
		data = (char *)data + (index * etype->size);
		break;
	}

	case RTTI_TYPE_LIST:
	case RTTI_TYPE_STRUCT: {
		const rtti_field_t *field;

		/* find field */
		for (field = type->args[0].v; field->name != NULL; field++) {
			const size_t fnlen = str_len(field->name);

			/* Handle field names with separator in them */
			if (str_cmpn(name, field->name, fnlen) == 0 &&
					(name[fnlen] == '\0' ||
					 name[fnlen] == RTTI_SEPARATOR)) {
				next = (name[fnlen] != '\0') ? name + fnlen + 1 : NULL;
				break;
			}
		}

		if (field->name == NULL) {
			error_set(ENOENT, "struct member not found (%s)", name);
			return NULL;
		}

		type = field->type;
		data = (char *)data + field->offset;
		break;
	}

	default:
		error_set(EINVAL, "invalid type class (%d)", type->tclass);
		return NULL;
	}

	/* recurse on sub-element */
	if ((type = rtti_find(type, next, &data)) == NULL)
		return NULL;

	/* done */
	*datap = data;
	return type;
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

void rtti_region_init(const rtti_t *type, void *data)
{
	mem_set(data, 0, type->size);
}

void rtti_region_copy(const rtti_t *type, const void *src, void *dst)
{
	mem_cpy(dst, src, type->size);
}

bool rtti_region_equal(const rtti_t *type, const void *a, const void *b)
{
	return mem_cmp(a, b, type->size) == 0;
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

void rtti_nothing_free(const rtti_t *type, void *data)
{
}
