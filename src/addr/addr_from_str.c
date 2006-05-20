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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#include "addr/addr.h"

int addr_from_str(char *str, uint32_t *ip, uint32_t *mask)
{
	struct in_addr ib;
	char *addr_ip, *addr_mask;
	
	*ip   = 0;
	*mask = 0;
	
	addr_ip   = strtok(str, "/");
	addr_mask = strtok(NULL, "/");
	
	if (addr_ip == 0)
		return -1;
	
	if (inet_aton(addr_ip, &ib) == -1)
		return -1;
	
	*ip = ib.s_addr;
	
	if (addr_mask == 0) {
		/* default to /24 */
		*mask = ntohl(0xffffff00);
	} else {
		if (strchr(addr_mask, '.') == 0) {
			/* We have CIDR notation */
			int sz = atoi(addr_mask);
			
			for (*mask = 0; sz > 0; --sz) {
				*mask >>= 1;
				*mask  |= 0x80000000;
			}
			
			*mask = ntohl(*mask);
		} else {
			/* Standard netmask notation */
			if (inet_aton(addr_mask, &ib) == -1)
				return -1;
			
			*mask = ib.s_addr;
		}
	}
	
	return 0;
}
