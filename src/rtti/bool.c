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
#include <stdint.h>
#include <errno.h>

#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

#define REDUCE(D) (((D) != 0) ? 1 : 0)

static
int rtti_bool_reduce(const rtti_t *type, const void *data)
{
	switch (type->size) {
	case 1: return REDUCE(CAST(uint8_t,  data)); break;
	case 2: return REDUCE(CAST(uint16_t, data)); break;
	case 3: return REDUCE(CAST(uint32_t, data)); break;
	case 4: return REDUCE(CAST(uint64_t, data)); break;
	}

	return 0;
}

int rtti_bool_equal(const rtti_t *type, const void *a, const void *b)
{
	int atruth = rtti_bool_reduce(type, a);
	int btruth = rtti_bool_reduce(type, b);

	return atruth == btruth;
}

int rtti_bool_encode(const rtti_t *type, const void *data, char **buf)
{
	int truth = rtti_bool_reduce(type, data);

	if (truth)
		_lucid_asprintf(buf, "true");
	else
		_lucid_asprintf(buf, "false");

	return str_len(*buf);
}

int rtti_bool_decode(const rtti_t *type, const char *buf, void *data)
{
	int truth;

	if (str_cmpn(buf, "true", 4) == 0)
		truth = 1;
	else if (str_cmpn(buf, "false", 5) == 0)
		truth = 0;
	else
		return errno = EINVAL, 0;

	switch (type->size) {
	case 1: CAST(uint8_t,  data) = truth; break;
	case 2: CAST(uint16_t, data) = truth; break;
	case 3: CAST(uint32_t, data) = truth; break;
	case 4: CAST(uint64_t, data) = truth; break;
	default: return errno = EDOM, -1;
	}

	PARSE_OK(buf, buf + (truth ? 4 : 5));
}
