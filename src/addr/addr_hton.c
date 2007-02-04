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

static inline
int islitend(void) {
	int i = 0;
	((char *)(&i))[0] = 1;
	return i == 1;
}

uint32_t addr_hton(uint32_t addr)
{
	if (islitend())
		return (addr >> 24) |
			((addr & 0xff0000) >> 8) |
			((addr & 0xff00  ) << 8) |
			(addr << 24);
	else
		return addr;
}
