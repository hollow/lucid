// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#include "addr.h"
#include "printf.h"

char *addr_to_str(uint32_t ip, uint32_t mask)
{
	char *buf;
	
	char *ipp   = (char *) &ip;
	char *maskp = (char *) &mask;
	
	if (mask)
		_lucid_asprintf(&buf, "%hhu.%hhu.%hhu.%hhu/%hhu.%hhu.%hhu.%hhu",
		                      ipp[0],   ipp[1],   ipp[2],   ipp[3],
		                      maskp[0], maskp[1], maskp[2], maskp[3]);
	
	else
		_lucid_asprintf(&buf, "%hhu.%hhu.%hhu.%hhu", ipp[0],  ipp[1],  ipp[2],  ipp[3]);
	
	return buf;
}
