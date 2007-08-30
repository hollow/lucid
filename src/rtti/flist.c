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

#include "flist.h"
#include "mem.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

int rtti_flist32_encode(const rtti_t *type, const void *data, char **buf)
{
	const flist32_t *list = type->args[0].v;
	const flag32_t *flag32 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;
	char *p;
	int rc;

	p = flist32_encode(list, flag32, clmod, delim);

	if (p)
		rc = _lucid_asprintf(buf, "\"%s\"", p);
	else
		rc = -1;

	mem_free(p);

	return rc;
}

int rtti_flist64_encode(const rtti_t *type, const void *data, char **buf)
{
	const flist64_t *list = type->args[0].v;
	const flag64_t *flag64 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;
	char *p;
	int rc;

	p = flist64_encode(list, flag64, clmod, delim);

	if (p)
		rc = _lucid_asprintf(buf, "\"%s\"", p);
	else
		rc = -1;

	mem_free(p);

	return rc;
}

int rtti_flist32_decode(const rtti_t *type, const char *buf, void *data)
{
	const flist32_t *list = type->args[0].v;
	flag32_t *flag32 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;
	const char *p = buf;
	char *sbuf;
	int res;

	SKIP_SPACE(p);

	if (str_cmpn(p, "null", 4) == 0) {
		flag32->flag = flag32->mask = 0;
		PARSE_OK(buf, p + 4);
	}

	errno = 0;
	if ((res = rtti_string_parse(p, &sbuf)) == -1)
		return -1;
	else
		p += res;

	if (errno)
		PARSE_ERROR(errno, buf, p);

	if (flist32_decode(sbuf, list, flag32, clmod, delim) == -1)
		PARSE_ERROR(errno, buf, p);

	PARSE_OK(buf, p);
}

int rtti_flist64_decode(const rtti_t *type, const char *buf, void *data)
{
	const flist64_t *list = type->args[0].v;
	flag64_t *flag64 = data;
	const char *delim = type->args[1].s;
	char clmod = (char)type->args[2].i;
	const char *p = buf;
	char *sbuf;
	int res;

	SKIP_SPACE(p);

	if (str_cmpn(p, "null", 4) == 0) {
		flag64->flag = flag64->mask = 0;
		PARSE_OK(buf, p + 4);
	}

	errno = 0;
	if ((res = rtti_string_parse(p, &sbuf)) == -1)
		return -1;
	else
		p += res;

	if (errno)
		PARSE_ERROR(errno, buf, p);

	if (flist64_decode(sbuf, list, flag64, clmod, delim) == -1)
		PARSE_ERROR(errno, buf, p);

	PARSE_OK(buf, p);
}
