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
#include <inttypes.h>

#include "printf.h"
#include "rtti.h"
#include "str.h"

#include "internal.h"

int rtti_int_encode(const rtti_t *type, const void *data, char **buf)
{
	const char *fmt = type->args[0].s;

	switch (type->size * 10 + type->args[2].i) {
	case 10: return _lucid_asprintf(buf, fmt, CAST(uint8_t, data));  break;
	case 11: return _lucid_asprintf(buf, fmt, CAST(int8_t, data));   break;
	case 20: return _lucid_asprintf(buf, fmt, CAST(uint16_t, data)); break;
	case 21: return _lucid_asprintf(buf, fmt, CAST(int16_t, data));  break;
	case 30: return _lucid_asprintf(buf, fmt, CAST(uint32_t, data)); break;
	case 31: return _lucid_asprintf(buf, fmt, CAST(int32_t, data));  break;
	case 40: return _lucid_asprintf(buf, fmt, CAST(uint64_t, data)); break;
	case 41: return _lucid_asprintf(buf, fmt, CAST(int64_t, data));  break;
	}
	
	return errno = EDOM, -1;
}

int rtti_int_decode(const rtti_t *type, const char *buf, void *data)
{
	const char *p = buf;
	int res;
	unsigned long long int value;

	if (type->init(type, data) == -1)
		return -1;

	if ((res = str_toumax(buf, &value, 0, str_len(buf))) == 0)
		PARSE_ERROR(EINVAL, buf, p);
	else
		p += res;

	switch (type->size * 10 + type->args[2].i) {
	case 10: CAST(uint8_t, data)  = value; break;
	case 11: CAST(int8_t, data)   = value; break;
	case 20: CAST(uint16_t, data) = value; break;
	case 21: CAST(int16_t, data)  = value; break;
	case 30: CAST(uint32_t, data) = value; break;
	case 31: CAST(int32_t, data)  = value; break;
	case 40: CAST(uint64_t, data) = value; break;
	case 41: CAST(int64_t, data)  = value; break;
	default: return errno = EDOM, -1;
	}

	PARSE_OK(buf, p);
}
