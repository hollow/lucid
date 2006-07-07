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

void sha1_update(SHA1* context, unsigned char *data, unsigned int len)
{
	unsigned int i, j;
	
	j = (context->count[0] >> 3) & 63;
	
	if ((context->count[0] += len << 3) < (len << 3))
		context->count[1]++;
	
	context->count[1] += (len >> 29);
	
	if ((j + len) > 63) {
		memcpy(&context->buffer[j], data, (i = 64-j));
		
		sha1_transform(context->state, context->buffer);
		
		for (; i + 63 < len; i += 64)
			sha1_transform(context->state, &data[i]);
		
		j = 0;
	}
	
	else
		i = 0;
	
	memcpy(&context->buffer[j], &data[i], len - i);
}
