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

#include "mem.h"
#include "str.h"
#include "stralloc.h"
#include "whirlpool.h"

char *whirlpool_digest(const char *str)
{
	whirlpool_t ctx;
	stralloc_t sa;
	char *buf;
	uint8_t digest[DIGESTBYTES];
	int i;
	
	whirlpool_init(&ctx);
	whirlpool_add(&ctx, (const unsigned char * const) str, str_len(str)*8);
	whirlpool_finalize(&ctx, digest);
	
	stralloc_init(&sa);
	
	for (i = 0; i < DIGESTBYTES; i++)
		stralloc_catf(&sa, "%02X", digest[i]);
	
	buf = mem_dup(sa.s, sa.len);
	
	stralloc_free(&sa);
	
	return buf;
}
