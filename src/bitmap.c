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

#include "bitmap.h"

uint32_t i2v32(int index)
{
	if (index < 0 || index > 31)
		return 0;

	return (1UL << index);
}

uint64_t i2v64(int index)
{
	if (index < 0 || index > 63)
		return 0;

	return (1ULL << index);
}

int v2i32(uint32_t val)
{
	int index = 0;

	if (val == 0)
		return -1;

	while ((val = val >> 1))
		index++;

	return index;
}

int v2i64(uint64_t val)
{
	int index = 0;

	if (val == 0)
		return -1;

	while ((val = val >> 1))
		index++;

	return index;
}
