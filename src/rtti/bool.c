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

#include "error.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

#define REDUCE(D) (((D) == 0) ? false : true)

static
bool rtti_bool_reduce(const rtti_t *type, const void *data)
{
	switch (type->size) {
	case 1: return REDUCE(CAST(uint8_t,  data)); break;
	case 2: return REDUCE(CAST(uint16_t, data)); break;
	case 4: return REDUCE(CAST(uint32_t, data)); break;
	case 8: return REDUCE(CAST(uint64_t, data)); break;
	}

	return false;
}

bool rtti_bool_equal(const rtti_t *type, const void *a, const void *b)
{
	bool atruth = rtti_bool_reduce(type, a);
	bool btruth = rtti_bool_reduce(type, b);

	return atruth == btruth;
}

char *rtti_bool_encode(const rtti_t *type, const void *data)
{
	if(rtti_bool_reduce(type, data))
		return str_dup("true");

	return str_dup("false");
}

void rtti_bool_decode(const rtti_t *type, const char **buf, void *data)
{
	int truth;

	SKIP_SPACE(buf);

	if (str_cmpn(*buf, "true", 4) == 0)
		truth = true;
	else if (str_cmpn(*buf, "false", 5) == 0)
		truth = false;
	else {
		error_set(EILSEQ, "expected BOOL near '%.16s'", *buf);
		return;
	}

	switch (type->size) {
	case 1: CAST(uint8_t,  data) = truth; break;
	case 2: CAST(uint16_t, data) = truth; break;
	case 4: CAST(uint32_t, data) = truth; break;
	case 8: CAST(uint64_t, data) = truth; break;
	}

	*buf += truth ? 4 : 5;
}
