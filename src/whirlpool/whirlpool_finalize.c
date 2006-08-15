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

#include "whirlpool.h"

void whirlpool_finalize(whirlpool_t * const context, unsigned char * const result)
{
	int i;
	uint8_t *buffer    = context->buffer;
	uint8_t *bitLength = context->bitLength;
	int bufferBits     = context->bufferBits;
	int bufferPos      = context->bufferPos;
	uint8_t *digest    = result;
	
	/* append a '1'-bit */
	buffer[bufferPos] |= 0x80U >> (bufferBits & 7);
	bufferPos++; /* all remaining bits on the current uint8_t are set to zero. */
	
	/* pad with zero bits to complete (N*WBLOCKBITS - LENGTHBITS) bits */
	if (bufferPos > WBLOCKBYTES - LENGTHBYTES) {
		if (bufferPos < WBLOCKBYTES)
			memset(&buffer[bufferPos], 0, WBLOCKBYTES - bufferPos);
		
		/* process data block */
		whirlpool_transform(context);
		
		/* reset buffer */
		bufferPos = 0;
	}
	
	if (bufferPos < WBLOCKBYTES - LENGTHBYTES)
		memset(&buffer[bufferPos], 0, (WBLOCKBYTES - LENGTHBYTES) - bufferPos);
	
	bufferPos = WBLOCKBYTES - LENGTHBYTES;
	
	/* append bit length of hashed data */
	memcpy(&buffer[WBLOCKBYTES - LENGTHBYTES], bitLength, LENGTHBYTES);
	
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
	
	context->bufferBits = bufferBits;
	context->bufferPos  = bufferPos;
}
