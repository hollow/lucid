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
#include "cext.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

void rtti_pointer_init(const rtti_t *type, void *data)
{
	CHECK_TYPE(POINTER);
	CAST(void *, data) = NULL;
}

void rtti_pointer_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_t *ptype = type->args[0].v;
	const void *src_pdata = CAST(const void *, src);
	void *dst_pdata;

	CHECK_TYPE(POINTER);

	if (src_pdata == NULL)
		dst_pdata = NULL;

	else {
		dst_pdata = malloc(ptype->size);
		ptype->copy(ptype, src_pdata, dst_pdata);
		error_do return;
	}

	CAST(void *, dst) = dst_pdata;
}

bool rtti_pointer_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_t *ptype = type->args[0].v;
	const void *pdata1 = CAST(const void *, a);
	const void *pdata2 = CAST(const void *, b);

	CHECK_TYPE(POINTER);

	if (pdata1 && pdata2)
		return ptype->equal(ptype, pdata1, pdata2);
	else if (!pdata1 && !pdata2)
		return true;
	else
		return false;
}

char *rtti_pointer_encode(const rtti_t *type, const void *data)
{
	const rtti_t *ptype = type->args[0].v;
	const void *pdata = CAST(const void *, data);

	CHECK_TYPE(POINTER);

	if (pdata)
		return ptype->encode(ptype, pdata);

	return str_dup("null");
}

void rtti_pointer_decode(const rtti_t *type, const char **buf, void *data)
{
	const rtti_t *ptype = type->args[0].v;
	void *pdata;

	CHECK_TYPE(POINTER);

	if (str_cmpn(*buf, "null", 4) == 0) {
		pdata = NULL;
		*buf += 4;
	}

	else {
		pdata = malloc(ptype->size);
		ptype->decode(ptype, buf, pdata);
		error_do return;
	}

	CAST(void *, data) = pdata;
}
