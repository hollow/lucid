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

#define IDX(T, D, I) ((char *)(D) + ((I) * (T)->size))

int rtti_array_init(const rtti_t *type, void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++) {
		if (etype->init(etype, IDX(etype, data, i)) == -1) {
			while (i-- > 0)
				etype->uninit(etype, IDX(etype, data, i));
			return -1;
		}
	}

	return 0;
}

int rtti_array_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++) {
		const void *const src_elem = IDX(etype, src, i);
		void *const dst_elem = IDX(etype, dst, i);

		if (etype->copy(etype, src_elem, dst_elem) == -1)
			break;
	}

	if (i < asize) {
		while (i-- > 0)
			etype->uninit(etype, IDX(etype, dst, i));
		return -1;
	}

	return 0;
}

int rtti_array_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++) {
		if (!etype->equal(etype, IDX(etype, a, i), IDX(etype, b, i)))
			break;
	}

	return (i == asize);
}

int rtti_array_encode(const rtti_t *type, const void *data, char **buf)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	stralloc_t _rbuf, *rbuf = &_rbuf;
	char *ebuf;
	size_t i;

	CHECK_TYPE(ARRAY);

	*buf = NULL;
	stralloc_init(rbuf);
	stralloc_cats(rbuf, "[");

	for (i = 0; i < asize; i++) {
		if (etype->encode(etype, IDX(etype, data, i), &ebuf) == -1)
			goto out;

		stralloc_cats(rbuf, ebuf);
		mem_free(ebuf);

		if (i < (asize - 1))
			stralloc_cats(rbuf, ",");
	}

	stralloc_cats(rbuf, "]");
	*buf = stralloc_finalize(rbuf);

out:
	stralloc_free(rbuf);
	return *buf ? str_len(*buf) : -1;
}

int rtti_array_decode(const rtti_t *type, const char *buf, void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	const char *p = buf;
	size_t i = 0;
	int res;

	CHECK_TYPE(ARRAY);

	if (type->init(type, data) == -1)
		return -1;

	SKIP_SPACE(p);
	SKIP_CHAR(p, '[') {
		PARSE_ERROR(EINVAL, buf, p);
	}
	SKIP_SPACE(p);

	while (i < asize) {
		errno = 0;

		switch ((res = etype->decode(etype, p, IDX(etype, data, i)))) {
		case  0: PARSE_ERROR(EINVAL, buf, p);
		case -1: return -1;
		default: p += res;
		}

		if (errno)
			PARSE_ERROR(errno, buf, p);

		i++;

		SKIP_SPACE(p);
		SKIP_CHAR(p, ',') {
			break;
		}
	}

	SKIP_SPACE(p);
	SKIP_CHAR(p, ']') {
		PARSE_ERROR(EINVAL, buf, p);
	}

	PARSE_OK(buf, p);
}

int rtti_array_free(const rtti_t *type, void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++)
		etype->uninit(etype, IDX(etype, data, i));

	return 0;
}
