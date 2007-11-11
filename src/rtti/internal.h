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

#ifndef _RTTI_INTERNAL_H
#define _RTTI_INTERNAL_H

#include "char.h"

#define CAST(T, D) *((T *)(D))

#define CHECK_TYPE(T) \
	assert(type->tclass == RTTI_TYPE_ ## T)

static inline
void SKIP_SPACE(const char **buf)
{
	const char *p = *buf;

	while (true) {
		while (char_isspace(*p))
			p++;

		if (*p != '#')
			break;

		while (*p && *p != '\n')
			p++;

		if (*p == '\n')
			p++;
		else
			break;
	}

	*buf = p;
}

#define SKIP_CHAR(B, C) \
	if (**B == C) { (*B)++; } else

#endif
