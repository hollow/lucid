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
#include "flist.h"
#include "cext.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

char *rtti_flist32_encode(const rtti_t *type, const void *data)
{
	const flist32_t *list = type->args[0].v;
	const flag32_t *flag32 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;
	char *buf, *str = flist32_encode(list, flag32, clmod, delim);

	_lucid_asprintf(&buf, "\"%s\"", str);
	return buf;
}

char *rtti_flist64_encode(const rtti_t *type, const void *data)
{
	const flist64_t *list = type->args[0].v;
	const flag64_t *flag64 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;
	char *buf, *str = flist64_encode(list, flag64, clmod, delim);

	_lucid_asprintf(&buf, "\"%s\"", str);
	return buf;
}

void rtti_flist32_decode(const rtti_t *type, const char **buf, void *data)
{
	const flist32_t *list = type->args[0].v;
	flag32_t *flag32 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;

	SKIP_SPACE(buf);

	flag32->flag = flag32->mask = 0;

	if (str_cmpn(*buf, "null", 4) == 0) {
		*buf += 4;
		return;
	}

	char *sbuf = rtti_string_parse(buf);
	error_do return;

	if (flist32_decode(sbuf, list, flag32, clmod, delim) == -1) {
		error_set(errno, "failed to decode flist32");
		return;
	}
}

void rtti_flist64_decode(const rtti_t *type, const char **buf, void *data)
{
	const flist64_t *list = type->args[0].v;
	flag64_t *flag64 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;

	SKIP_SPACE(buf);

	flag64->flag = flag64->mask = 0;

	if (str_cmpn(*buf, "null", 4) == 0) {
		*buf += 4;
		return;
	}

	char *sbuf = rtti_string_parse(buf);
	error_do return;

	if (flist64_decode(sbuf, list, flag64, clmod, delim) == -1) {
		error_set(errno, "failed to decode flist64");
		return;
	}
}
