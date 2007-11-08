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
#include <inttypes.h>

#include "error.h"
#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

char *rtti_int_encode(const rtti_t *type, const void *data)
{
	char *buf = NULL;

	switch (type->size * 10 + type->args[0].i) {
	case 10: _lucid_asprintf(&buf, "%"PRIu8,  CAST(uint8_t, data));  break;
	case 11: _lucid_asprintf(&buf, "%"PRIi8,  CAST(int8_t, data));   break;
	case 20: _lucid_asprintf(&buf, "%"PRIu16, CAST(uint16_t, data)); break;
	case 21: _lucid_asprintf(&buf, "%"PRIi16, CAST(int16_t, data));  break;
	case 40: _lucid_asprintf(&buf, "%"PRIu32, CAST(uint32_t, data)); break;
	case 41: _lucid_asprintf(&buf, "%"PRIi32, CAST(int32_t, data));  break;
	case 80: _lucid_asprintf(&buf, "%"PRIu64, CAST(uint64_t, data)); break;
	case 81: _lucid_asprintf(&buf, "%"PRIi64, CAST(int64_t, data));  break;
	default: assert_not_reached();
	}

	return buf;
}

void rtti_int_decode(const rtti_t *type, const char **buf, void *data)
{
	unsigned long long int value;
	int res;

	if ((res = str_toumax(*buf, &value, 0, str_len(*buf))) == 0) {
		error_set(errno, "failed to decode int near '%.16s'", *buf);
		return;
	}

	switch (type->size * 10 + type->args[0].i) {
	case 10: CAST(uint8_t, data)  = value; break;
	case 11: CAST(int8_t, data)   = value; break;
	case 20: CAST(uint16_t, data) = value; break;
	case 21: CAST(int16_t, data)  = value; break;
	case 40: CAST(uint32_t, data) = value; break;
	case 41: CAST(int32_t, data)  = value; break;
	case 80: CAST(uint64_t, data) = value; break;
	case 81: CAST(int64_t, data)  = value; break;
	default: assert_not_reached();
	}

	*buf += res;
}
