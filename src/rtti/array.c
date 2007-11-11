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
#include "stralloc.h"

#include "internal.h"

#define IDX(T, D, I) ((char *)(D) + ((I) * (T)->size))

void rtti_array_init(const rtti_t *type, void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++) {
		etype->init(etype, IDX(etype, data, i));
		error_dof("failed to initialize array (%s)", etype->name) {
			while (i-- > 0)
				etype->uninit(etype, IDX(etype, data, i));
			return;
		}
	}
}

void rtti_array_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++) {
		const void *const src_elem = IDX(etype, src, i);
		void *const dst_elem = IDX(etype, dst, i);

		etype->copy(etype, src_elem, dst_elem);
		error_dof("failed to copy array (%s)", etype->name) {
			while (i-- > 0)
				etype->uninit(etype, IDX(etype, dst, i));
			return;
		}
	}
}

bool rtti_array_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	CHECK_TYPE(ARRAY);

	for (i = 0; i < asize; i++) {
		bool eq = etype->equal(etype, IDX(etype, a, i), IDX(etype, b, i));
		error_dof("failed to compare array (%s)", etype->name)
			return false;

		if (!eq)
			return false;
	}

	return true;
}

char *rtti_array_encode(const rtti_t *type, const void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	stralloc_t _rbuf, *rbuf = &_rbuf;
	char *ebuf, *buf = NULL;
	size_t i;

	CHECK_TYPE(ARRAY);

	stralloc_init(rbuf);
	stralloc_cats(rbuf, "[");

	for (i = 0; i < asize; i++) {
		ebuf = etype->encode(etype, IDX(etype, data, i));
		error_dof("failed to encode array (%s)", etype->name)
			goto out;

		stralloc_cats(rbuf, ebuf);
		mem_free(ebuf);

		if (i < (asize - 1))
			stralloc_cats(rbuf, ",");
	}

	stralloc_cats(rbuf, "]");
	buf = stralloc_finalize(rbuf);

out:
	stralloc_free(rbuf);
	return buf;
}

void rtti_array_decode(const rtti_t *type, const char **buf, void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i = 0;

	CHECK_TYPE(ARRAY);

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, '[') {
		error_set(EILSEQ, "expected LSQBR near '%.16s'", *buf);
		return;
	}

	while (i < asize) {
		etype->decode(etype, buf, IDX(etype, data, i));
		error_dof("failed to decode array (%s)", etype->name)
			return;

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, ',') {
			break;
		}
	}

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, ']') {
		error_set(EILSEQ, "expected RSQBR near '%.16s'", *buf);
		return;
	}
}

void rtti_array_free(const rtti_t *type, void *data)
{
	const rtti_t *const etype = type->args[0].v;
	const size_t asize = type->args[1].i;
	size_t i;

	for (i = 0; i < asize; i++)
		etype->uninit(etype, IDX(etype, data, i));
}
