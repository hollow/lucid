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

#include "internal.h"

int rtti_pointer_init(const rtti_t *type, void *data)
{
	const rtti_t *const ptype = type->args[0].v;
	void *pdata;

	if ((pdata = mem_alloc(ptype->size)) == NULL)
		return -1;

	if (ptype->init(ptype, pdata) == -1)
		return -1;

	CAST(void *, data) = pdata;
	return 0;
}

int rtti_pointer_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_t *const ptype = type->args[0].v;
	void *const src_pdata = CAST(void *, src);
	void *dst_pdata;

	if ((dst_pdata = mem_alloc(ptype->size)) == NULL)
		return -1;

	if (ptype->copy(ptype, src_pdata, dst_pdata) == -1)
		return -1;

	CAST(void *, dst) = dst_pdata;
	return 0;
}

int rtti_pointer_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_t *const ptype = type->args[0].v;
	void *const pdata1 = CAST(void *, a);
	void *const pdata2 = CAST(void *, b);

	return ptype->equal(ptype, pdata1, pdata2);
}

int rtti_pointer_encode(const rtti_t *type, const void *data, char **buf)
{
	const rtti_t *const ptype = type->args[0].v;
	void *const pdata = CAST(void *, data);

	return ptype->encode(ptype, pdata, buf);
}

int rtti_pointer_decode(const rtti_t *type, const char *buf, void *data)
{
	const rtti_t *const ptype = type->args[0].v;
	const char *p = buf;
	void *pdata;
	int res;

	if ((pdata = mem_alloc(ptype->size)) == NULL)
		return -1;

	if ((res = ptype->decode(ptype, buf, pdata)) == -1)
		return -1;

	p += res;
	CAST(void *, data) = pdata;
	PARSE_OK(buf, p);
}

int rtti_pointer_free(const rtti_t *type, void *data)
{
	const rtti_t *const ptype = type->args[0].v;
	void *const pdata = CAST(void *, data);

	ptype->uninit(ptype, pdata);
	mem_free(pdata);
	CAST(void *, data) = NULL;

	return 0;
}
