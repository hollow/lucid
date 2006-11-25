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
#include "printf.h"

char *addr_to_str(uint32_t ip, uint32_t mask)
{
	char *buf;
	
	ip   = addr_ntoh(ip);
	mask = addr_ntoh(mask);
	
	uint8_t *ipp   = (uint8_t *) &ip;
	uint8_t *maskp = (uint8_t *) &mask;
	
	if (mask)
		_lucid_asprintf(&buf, "%u.%u.%u.%u/%u.%u.%u.%u",
		                      ipp[0],   ipp[1],   ipp[2],   ipp[3],
		                      maskp[0], maskp[1], maskp[2], maskp[3]);
	
	else
		_lucid_asprintf(&buf, "%u.%u.%u.%u", ipp[0],  ipp[1],  ipp[2],  ipp[3]);
	
	return buf;
}
