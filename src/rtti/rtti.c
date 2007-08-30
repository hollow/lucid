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
#include <string.h>

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
	rtti_notsupp_decode,
	rtti_nothing_free,
	{ { (void *)2 }, { (void *)0 } }    /* args for structs_int_ascify */
};

int rtti_init(const rtti_t *type, const char *name, void *data)
{
	/* find item if any */
	if ((type = rtti_find(type, name, &data)) == NULL)
		return -1;

	/* initialize data */
	return type->init(type, data);
}

int rtti_free(const rtti_t *type, const char *name, void *data)
{
	const int errno_save = errno;

	/* find item if any */
	if ((type = rtti_find(type, name, &data)) == NULL)
		return -1;

	type->uninit(type, data);
	return errno = errno_save, 0;
}

int rtti_get(const rtti_t *type, const char *name,
		const void *src, void *dst)
{
	/* find item if any */
	if ((type = rtti_find(type, name, (void **)&src)) == NULL)
		return -1;

	return type->copy(type, src, dst);
}

int rtti_set(const rtti_t *type, const void *src,
		const char *name, void *dst)
{
	/* find item if any */
	if ((type = rtti_find(type, name, &dst)) == NULL)
		return -1;

	/* make a copy of src */
	void *copy;
	if ((copy = mem_alloc(type->size)) == NULL)
		return -1;
	if (type->copy(type, src, copy) == -1) {
		mem_free(copy);
		return -1;
	}

	/* free dst item */
	type->uninit(type, dst);

	/* set copy in dst */
	mem_cpy(dst, copy, type->size);

	mem_free(copy);
	return 0;
}

int rtti_encode(const rtti_t *type, const char *name,
		const void *data, char **buf)
{
	/* find item if any */
	if ((type = rtti_find(type, name, (void **)&data)) == NULL)
		return -1;

	return type->encode(type, data, buf);
}

int rtti_decode(const rtti_t *type, const char *name,
		const char *buf, void *data)
{
	/* find item if any */
	if ((type = rtti_find(type, name, &data)) == NULL)
		return -1;

	return type->decode(type, buf, data);
}

int rtti_compare(const rtti_t *type, const char *name,
		const void *data1, const void *data2)
{
	/* find item if any */
	if (rtti_find(type, name, (void **)&data1) == NULL)
		return -1;
	if ((type = rtti_find(type, name, (void **)&data2)) == NULL)
		return -1;

	return type->equal(type, data1, data2);
}

const rtti_t *rtti_find(const rtti_t *type, const char *name, void **datap)
{
	void *data = *datap;
	const char *next;

	/* stop recursing on empty string */
	if (name == NULL || *name == '\0')
		return type;

	/* primitive types don't have sub-elements */
	if (type->tclass == RTTI_TYPE_PRIMITIVE)
		return errno = ENOENT, NULL;

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
				(*eptr != '\0' && *eptr != RTTI_SEPARATOR))
			return errno = ENOENT, NULL;

		if (index >= length)
			return errno = EDOM, NULL;

		type = etype;
		data = (char *)data + (index * etype->size);
		break;
	}

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

		if (field->name == NULL)
			return errno = ENOENT, NULL;

		type = field->type;
		data = (char *)data + field->offset;
		break;
	}

	default:
		return errno = EINVAL, NULL;
	}

	/* recurse on sub-element */
	if ((type = rtti_find(type, next, &data)) == NULL)
		return NULL;

	/* done */
	*datap = data;
	return type;
}

int rtti_region_init(const rtti_t *type, void *data)
{
	mem_set(data, 0, type->size);
	return 0;
}

int rtti_region_copy(const rtti_t *type, const void *src, void *dst)
{
	mem_cpy(dst, src, type->size);
	return 0;
}

int rtti_region_equal(const rtti_t *type, const void *a, const void *b)
{
	return mem_cmp(a, b, type->size) == 0;
}

int rtti_notsupp_init(const rtti_t *type, void *data)
{
	return errno = ENOTSUP, -1;
}

int rtti_notsupp_copy(const rtti_t *type, const void *src, void *dst)
{
	return errno = ENOTSUP, -1;
}

int rtti_notsupp_equal(const rtti_t *type, const void *a, const void *b)
{
	return errno = ENOTSUP, -1;
}

int rtti_notsupp_encode(const rtti_t *type, const void *data, char **buf)
{
	return errno = ENOTSUP, -1;
}

int rtti_notsupp_decode(const rtti_t *type, const char *buf, void *data)
{
	return errno = ENOTSUP, -1;
}

int rtti_nothing_free(const rtti_t *type, void *data)
{
	return 0;
}
