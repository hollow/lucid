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
#include "base64.h"
#include "cext.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

void rtti_data_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_data_t *sdata = src;
	rtti_data_t *ddata = dst;

	if (sdata->length == 0) {
		memset(ddata, 0, sizeof(*ddata));
		return;
	}

	ddata->data = memdup(sdata->data, sdata->length);
	ddata->length = sdata->length;
}

bool rtti_data_equal(const rtti_t *type, const void *a, const void *b)
{
	const rtti_data_t *adata = a;
	const rtti_data_t *bdata = b;

	return (adata->length == bdata->length &&
			memcmp(adata->data, bdata->data, adata->length) == 0);
}

char *rtti_data_encode(const rtti_t *type, const void *data)
{
	const rtti_data_t *d = data;

	if (d->length == 0)
		return str_dup("null");

	char *ebuf;
	if ((ebuf = base64_encode(d->data, d->length)) == NULL) {
		error_set(errno, "failed to encode binary data");
		return NULL;
	}

	char *buf = NULL;
	_lucid_asprintf(&buf, "\"%s\"", ebuf);
	return buf;
}

void rtti_data_decode(const rtti_t *type, const char **buf, void *data)
{
	SKIP_SPACE(buf);

	rtti_data_t *d = data;
	d->data   = NULL;
	d->length = 0;

	if (str_cmpn(*buf, "null", 4) == 0) {
		*buf += 4;
		return;
	}

	char *sbuf = rtti_string_parse(buf);
	error_do return;

	if ((d->data = base64_decode(sbuf, &(d->length))) == NULL) {
		error_set(errno, "failed to decode binary data");
		return;
	}
}
