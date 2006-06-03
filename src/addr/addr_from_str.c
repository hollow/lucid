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
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include "addr/addr.h"
#include "str/str.h"

int addr_from_str(const char *str, uint32_t *ip, uint32_t *mask)
{
	struct in_addr ib;
	char *o, *buf, *addr_ip, *addr_mask;
	
	if (str_isempty(str))
		return errno = EINVAL, -1;
	
	buf = o = strdup(str);
	
	addr_ip   = strtok(buf,  "/");
	addr_mask = strtok(NULL, "/");
	
	if (str_isempty(addr_ip)) {
		errno = EINVAL;
		goto err;
	}
	
	if (inet_aton(addr_ip, &ib) == -1)
		goto err;
	
	*ip = ib.s_addr;
	
	if (str_isempty(addr_mask)) /* default to /24 */
		*mask = ntohl(0xffffff00);
	
	else if (str_isdigit(addr_mask)) { /* We have CIDR notation */
		int sz = atoi(addr_mask);
		
		for (*mask = 0; sz > 0; --sz) {
			*mask >>= 1;
			*mask  |= 0x80000000;
		}
		
		*mask = ntohl(*mask);
	}
	
	else { /* Standard netmask notation */
		if (inet_aton(addr_mask, &ib) == -1)
			goto err;
		
		*mask = ib.s_addr;
	}
	
	free(o);
	return 0;
	
err:
	free(o);
	return -1;
}
