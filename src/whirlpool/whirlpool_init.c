// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
//
// The Whirlpool algorithm was developed by
//                Paulo S. L. M. Barreto <pbarreto@scopus.com.br> and
//                Vincent Rijmen <vincent.rijmen@cryptomathic.com>
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

#include "mem.h"
#include "whirlpool.h"

void whirlpool_init(whirlpool_t * const context)
{
	int i;

	mem_set(context->len, 0, LENGTHBYTES);

	context->bits = context->pos = 0;
	context->buf[0]  = 0;

	for (i = 0; i < 8; i++)
		context->hash[i] = 0L;
}
