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

#include <stdlib.h>
#include <errno.h>

#include "rtti.h"

int rtti_float_equal(const rtti_t *type, const void *a, const void *b)
{
	return errno = ENOTSUP, -1;
}

int rtti_float_encode(const rtti_t *type, const void *data, char **buf)
{
	return errno = ENOTSUP, -1;
}

int rtti_float_decode(const rtti_t *type, const char *buf, void *data)
{
	return errno = ENOTSUP, -1;
}
