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

void whirlpool_finalize(whirlpool_t * const context, unsigned char * const result)
{
	int i;
	uint8_t *buf    = context->buf;
	uint8_t *len    = context->len;
	int bits        = context->bits;
	int pos         = context->pos;
	uint8_t *digest = result;
	
	/* append a '1'-bit */
	buf[pos] |= 0x80U >> (bits & 7);
	pos++; /* all remaining bits on the current uint8_t are set to zero. */
	
	/* pad with zero bits to complete (N*WBLOCKBITS - LENGTHBITS) bits */
	if (pos > WBLOCKBYTES - LENGTHBYTES) {
		if (pos < WBLOCKBYTES)
			mem_set(&buf[pos], 0, WBLOCKBYTES - pos);
		
		/* process data block */
		whirlpool_transform(context);
		
		/* reset buffer */
		pos = 0;
	}
	
	if (pos < WBLOCKBYTES - LENGTHBYTES)
		mem_set(&buf[pos], 0, (WBLOCKBYTES - LENGTHBYTES) - pos);
	
	pos = WBLOCKBYTES - LENGTHBYTES;
	
	/* append bit length of hashed data */
	mem_cpy(&buf[WBLOCKBYTES - LENGTHBYTES], len, LENGTHBYTES);
	
	/* process data block */
	whirlpool_transform(context);
	
	/* return the completed message digest */
	for (i = 0; i < DIGESTBYTES/8; i++) {
		digest[0] = (uint8_t)(context->hash[i] >> 56);
		digest[1] = (uint8_t)(context->hash[i] >> 48);
		digest[2] = (uint8_t)(context->hash[i] >> 40);
		digest[3] = (uint8_t)(context->hash[i] >> 32);
		digest[4] = (uint8_t)(context->hash[i] >> 24);
		digest[5] = (uint8_t)(context->hash[i] >> 16);
		digest[6] = (uint8_t)(context->hash[i] >>  8);
		digest[7] = (uint8_t)(context->hash[i]      );
		digest += 8;
	}
	
	context->bits = bits;
	context->pos  = pos;
}
