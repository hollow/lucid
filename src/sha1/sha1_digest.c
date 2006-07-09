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
#include <string.h>

#include "sha1/sha1.h"
#include "stralloc/stralloc.h"

char *sha1_digest(const char *str)
{
	sha1_t ctx;
	stralloc_t sa;
	char *buf;
	unsigned char digest[20], *dat;
	unsigned int i, j, len;
	
	len = strlen(str);
	dat = malloc(len + 1);
	
	bzero(dat, len + 1);
	memcpy(dat, str, len);
	
	sha1_init(&ctx);
	sha1_update(&ctx, dat, len);
	sha1_final(digest, &ctx);
	
	stralloc_init(&sa);
	
	for (i = 0; i < 5; i++)
		for (j = 0; j < 4; j++)
			stralloc_catf(&sa, "%02X", digest[i*4+j]);
	
	buf = strndup(sa.s, sa.len);
	
	stralloc_free(&sa);
	free(dat);
	
	return buf;
}
