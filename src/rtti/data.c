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
#include "base64.h"
#include "mem.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

void rtti_data_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_data_t *const sdata = src;
	rtti_data_t *const ddata = dst;

	if (sdata->length == 0) {
		mem_set(ddata, 0, sizeof(*ddata));
		return;
	}

	ddata->data = mem_dup(sdata->data, sdata->length);
	ddata->length = sdata->length;
}

bool rtti_data_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_data_t *const d1 = a;
	const rtti_data_t *const d2 = b;

	return (d1->length == d2->length &&
			mem_cmp(d1->data, d2->data, d1->length) == 0);
}

char *rtti_data_encode(const rtti_t *type, const void *data)
{
	const rtti_data_t *const d = data;
	char *ebuf;

	if (d->length == 0)
		return str_dup("null");

	if ((ebuf = base64_encode(d->data, d->length)) == NULL) {
		error_set(errno, "failed to encode binary data");
		return NULL;
	}

	char *buf = NULL;
	_lucid_asprintf(&buf, "\"%s\"", ebuf);
	mem_free(ebuf);

	return buf;
}

void rtti_data_decode(const rtti_t *type, const char **buf, void *data)
{
	rtti_data_t *const d = data;
	char *sbuf;

	SKIP_SPACE(buf);

	d->data   = NULL;
	d->length = 0;

	if (str_cmpn(*buf, "null", 4) == 0) {
		*buf += 4;
		return;
	}

	sbuf = rtti_string_parse(buf);
	error_do return;

	if ((d->data = base64_decode(sbuf, &(d->length))) == NULL) {
		error_set(errno, "failed to decode binary data");
		return;
	}
}

void rtti_data_free(const rtti_t *type, void *data)
{
	rtti_data_t *const d = data;

	mem_free(d->data);
	mem_set(d, 0, sizeof(*d));
}
