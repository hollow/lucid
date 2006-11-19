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

#include "addr.h"
#include "str.h"

int addr_from_str(const char *str, uint32_t *ip, uint32_t *mask)
{
	struct in_addr ib;
	char *p, *addr_ip, *addr_mask;
	int errno_orig;
	
	if (str_isempty(str))
		return errno = EINVAL, -1;
	
	addr_ip   = str_dup(str);
	addr_mask = NULL;
	
	p = str_index(addr_ip, '/', str_len(addr_ip));
	
	if (p && p - addr_ip > 0) {
		*p++ = '\0';
		addr_mask = p;
	}
	
	if (str_isempty(addr_ip)) {
		errno = EINVAL;
		goto err;
	}
	
	if (inet_aton(addr_ip, &ib) == 0) {
		errno = EINVAL;
		goto err;
	}
	
	*ip = ib.s_addr;
	
	if (str_isempty(addr_mask)) /* default to /24 */
		*mask = ntohl(0xffffff00);
	
	else if (str_isdigit(addr_mask)) { /* We have CIDR notation */
		int sz = atoi(addr_mask);
		
		if (sz < 0 || sz > 32) {
			errno = EINVAL;
			goto err;
		}
		
		for (*mask = 0; sz > 0; --sz) {
			*mask >>= 1;
			*mask  |= 0x80000000;
		}
		
		*mask = ntohl(*mask);
	}
	
	else { /* Standard netmask notation */
		if (inet_aton(addr_mask, &ib) == 0) {
			errno = EINVAL;
			goto err;
		}
		
		*mask = ib.s_addr;
	}
	
	free(addr_ip);
	return 0;
	
err:
	errno_orig = errno;
	free(addr_ip);
	errno = errno_orig;
	return -1;
}
