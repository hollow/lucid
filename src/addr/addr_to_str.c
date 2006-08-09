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

#include <string.h>
#include <arpa/inet.h>

#include "addr.h"
#include "stralloc.h"

char *addr_to_str(uint32_t ip, uint32_t mask)
{
	struct in_addr ib;
	char *str, *addr_ip, *addr_mask;
	stralloc_t addr;
	
	stralloc_init(&addr);
	
	ib.s_addr = ip;
	addr_ip = inet_ntoa(ib);
	stralloc_cats(&addr, addr_ip);
	
	if (mask > 0) {
		ib.s_addr = mask;
		addr_mask = inet_ntoa(ib);
		
		stralloc_catm(&addr, "/", addr_mask, NULL);
	}	
	
	str = strndup(addr.s, addr.len);
	stralloc_free(&addr);
	
	return str;
}
