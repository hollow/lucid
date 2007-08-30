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

#include "base64.h"
#include "mem.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

int rtti_data_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_data_t *const sdata = src;
	rtti_data_t *const ddata = dst;
	unsigned char *copy;

	if (sdata->length == 0) {
		mem_set(ddata, 0, sizeof(*ddata));
		return 0;
	}

	if ((copy = mem_alloc(sdata->length)) == NULL)
		return -1;

	mem_cpy(copy, sdata->data, sdata->length);
	ddata->data = copy;
	ddata->length = sdata->length;

	return 0;
}

int rtti_data_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_data_t *const d1 = a;
	const rtti_data_t *const d2 = b;

	return (d1->length == d2->length &&
			mem_cmp(d1->data, d2->data, d1->length) == 0);
}

int rtti_data_encode(const rtti_t *type, const void *data, char **buf)
{
	const rtti_data_t *const d = data;
	char *ebuf;
	int res;

	if (d->length == 0)
		return _lucid_asprintf(buf, "null");

	if ((ebuf = base64_encode(d->data, d->length)) == NULL)
		return -1;

	res = _lucid_asprintf(buf, "\"%s\"", ebuf);
	mem_free(ebuf);
	return res;
}

int rtti_data_decode(const rtti_t *type, const char *buf, void *data)
{
	rtti_data_t *const d = data;
	const char *p = buf;
	char *sbuf;
	int res;

	SKIP_SPACE(p);

	if (str_cmpn(p, "null", 4) == 0) {
		d->length = 0;
		d->data = NULL;
		PARSE_OK(buf, p + 4);
	}

	errno = 0;
	if ((res = rtti_string_parse(p, &sbuf)) == -1)
		return -1;
	else
		p += res;

	if (errno)
		PARSE_ERROR(errno, buf, p);

	if ((d->data = base64_decode(sbuf)) == NULL)
		PARSE_ERROR(EINVAL, buf, p);

	d->length = res;
	PARSE_OK(buf, p);
}

int rtti_data_free(const rtti_t *type, void *data)
{
	rtti_data_t *const d = data;

	mem_free(d->data);
	mem_set(d, 0, sizeof(*d));

	return 0;
}
