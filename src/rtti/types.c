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

#include <stdint.h>
#include <netinet/in.h>

#include "rtti.h"

rtti_t rtti_bool_type     = RTTI_BOOL_TYPE(int);
rtti_t rtti_data_type     = RTTI_DATA_TYPE;
rtti_t rtti_float_type    = RTTI_FLOAT_TYPE(float);
rtti_t rtti_double_type   = RTTI_FLOAT_TYPE(double);
rtti_t rtti_ldouble_type  = RTTI_FLOAT_TYPE(long double);
rtti_t rtti_int8_type     = RTTI_INT_TYPE(int8_t,   1);
rtti_t rtti_uint8_type    = RTTI_INT_TYPE(uint8_t,  0);
rtti_t rtti_int16_type    = RTTI_INT_TYPE(int16_t,  1);
rtti_t rtti_uint16_type   = RTTI_INT_TYPE(uint16_t, 0);
rtti_t rtti_int32_type    = RTTI_INT_TYPE(int32_t,  1);
rtti_t rtti_uint32_type   = RTTI_INT_TYPE(uint32_t, 0);
rtti_t rtti_int64_type    = RTTI_INT_TYPE(int64_t,  1);
rtti_t rtti_uint64_type   = RTTI_INT_TYPE(uint64_t, 0);
rtti_t rtti_bool_ptype    = RTTI_POINTER_TYPE(&rtti_bool_type);
rtti_t rtti_data_ptype    = RTTI_POINTER_TYPE(&rtti_data_type);
rtti_t rtti_float_ptype   = RTTI_POINTER_TYPE(&rtti_float_type);
rtti_t rtti_double_ptype  = RTTI_POINTER_TYPE(&rtti_double_type);
rtti_t rtti_ldouble_ptype = RTTI_POINTER_TYPE(&rtti_ldouble_type);
rtti_t rtti_int8_ptype    = RTTI_POINTER_TYPE(&rtti_int8_type);
rtti_t rtti_uint8_ptype   = RTTI_POINTER_TYPE(&rtti_uint8_type);
rtti_t rtti_int16_ptype   = RTTI_POINTER_TYPE(&rtti_int16_type);
rtti_t rtti_uint16_ptype  = RTTI_POINTER_TYPE(&rtti_uint16_type);
rtti_t rtti_int32_ptype   = RTTI_POINTER_TYPE(&rtti_int32_type);
rtti_t rtti_uint32_ptype  = RTTI_POINTER_TYPE(&rtti_uint32_type);
rtti_t rtti_int64_ptype   = RTTI_POINTER_TYPE(&rtti_int64_type);
rtti_t rtti_uint64_ptype  = RTTI_POINTER_TYPE(&rtti_uint64_type);
rtti_t rtti_string_ptype  = RTTI_POINTER_TYPE(&rtti_string_type);
rtti_t rtti_string_type   = RTTI_STRING_TYPE(1);

rtti_t rtti_inaddr_type = {
	sizeof(struct in_addr),
	"inaddr",
	RTTI_TYPE_PRIMITIVE,
	rtti_region_init,
	rtti_region_copy,
	rtti_notsup_equal,
	rtti_inaddr_encode,
	rtti_inaddr_decode,
	rtti_nothing_free,
	{ { NULL }, { NULL }, { NULL } }
};
