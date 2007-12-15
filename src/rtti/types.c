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

rtti_t rtti_bool_type     = RTTI_BOOL_TYPE(uint8);
rtti_t rtti_data_type     = RTTI_DATA_TYPE;
rtti_t rtti_int8_type     = RTTI_INT_TYPE(int8,   1);
rtti_t rtti_uint8_type    = RTTI_INT_TYPE(uint8,  0);
rtti_t rtti_int16_type    = RTTI_INT_TYPE(int16,  1);
rtti_t rtti_uint16_type   = RTTI_INT_TYPE(uint16, 0);
rtti_t rtti_int32_type    = RTTI_INT_TYPE(int32,  1);
rtti_t rtti_uint32_type   = RTTI_INT_TYPE(uint32, 0);
rtti_t rtti_int64_type    = RTTI_INT_TYPE(int64,  1);
rtti_t rtti_uint64_type   = RTTI_INT_TYPE(uint64, 0);
rtti_t rtti_bool_ptype    = RTTI_POINTER_TYPE(&rtti_bool_type);
rtti_t rtti_data_ptype    = RTTI_POINTER_TYPE(&rtti_data_type);
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

rtti_t rtti_in_addr_type = {
	sizeof(struct in_addr),
	"in_addr",
	RTTI_TYPE_PRIMITIVE,
	&ffi_type_pointer,
	rtti_region_init,
	rtti_region_copy,
	rtti_notsup_equal,
	rtti_in_addr_encode,
	rtti_in_addr_decode,
	{ { NULL }, { NULL }, { NULL } }
};

rtti_t rtti_port_type = {
	sizeof(uint16_t),
	"port",
	RTTI_TYPE_PRIMITIVE,
	&ffi_type_uint16,
	rtti_region_init,
	rtti_region_copy,
	rtti_region_equal,
	rtti_port_encode,
	rtti_port_decode,
	{ { (void *)(0) }, { NULL }, { NULL } }
};

rtti_t rtti_sockaddr_in_type = {
	sizeof(struct sockaddr_in),
	"sockaddr_in",
	RTTI_TYPE_PRIMITIVE,
	&ffi_type_pointer,
	rtti_region_init,
	rtti_region_copy,
	rtti_region_equal,
	rtti_sockaddr_in_encode,
	rtti_sockaddr_in_decode,
	{ { (void *)(0) }, { NULL }, { NULL } }
};
