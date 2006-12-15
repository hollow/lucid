// Copyright 2006 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "addr.h"
#include "scanf.h"
#include "str.h"

int addr_from_str(const char *str, uint32_t *ip, uint32_t *mask)
{
	int rc = 0;
	unsigned long long int cidr;
	
	union {
		uint8_t  b[4];
		uint32_t l;
	} u;
	
	const char *p = str_index(str, '/', str_len(str));
	
	if (ip && (!p || p - str > 0)) {
		if (_lucid_sscanf(str, "%hhu.%hhu.%hhu.%hhu",
		                  &u.b[0], &u.b[1], &u.b[2], &u.b[3]) == 4) {
			*ip = u.l;
			rc = 1;
		}
	}
	
	if (!p || !mask)
		return rc;
	
	p++;
	
	/* CIDR notation */
	if (!str_isempty(p) && str_isdigit(p)) {
		str_toumax(p, &cidr, 10, str_len(p));
		
		if (cidr > 0 && cidr <= 32) {
			*mask = addr_hton(0xffffffff & ~((1 << (32 - cidr)) - 1));
			rc   += 2;
		}
	}
	
	if (!str_isempty(p)) {
		if (_lucid_sscanf(p, "%hhu.%hhu.%hhu.%hhu",
		                  &u.b[0], &u.b[1], &u.b[2], &u.b[3]) == 4) {
			*mask = u.l;
			rc += 2;
		}
	}
	
	return rc;
}
