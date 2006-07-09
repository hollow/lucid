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

#include <string.h>

#include "sha1/sha1.h"

void sha1_final(unsigned char digest[20], sha1_t* context)
{
	unsigned long i, j;
	unsigned char finalcount[8];
	
	for (i = 0; i < 8; i++)
		finalcount[i] = (unsigned char)((context->count[(i >= 4 ? 0 : 1)] >> ((3-(i & 3)) * 8) ) & 255);
	
	sha1_update(context, (unsigned char *)"\200", 1);
	
	while ((context->count[0] & 504) != 448)
		sha1_update(context, (unsigned char *)"\0", 1);
	
	sha1_update(context, finalcount, 8);
	
	for (i = 0; i < 20; i++)
		digest[i] = (unsigned char)((context->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
	
	/* Wipe variables */
	i = j = 0;
	memset(context->buffer, 0, 64);
	memset(context->state, 0, 20);
	memset(context->count, 0, 8);
	memset(&finalcount, 0, 8);
	
	sha1_transform(context->state, context->buffer);
}
