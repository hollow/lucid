// Copyright 2006 Benedikt Böhm <hollow@gentoo.org>
//
// The Whirlpool algorithm was developed by
//                Paulo S. L. M. Barreto <pbarreto@scopus.com.br> and
//                Vincent Rijmen <vincent.rijmen@cryptomathic.com>
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

#include "whirlpool.h"

void whirlpool_add(whirlpool_t * const context,
                   const unsigned char * const source, unsigned long sourceBits)
{
	int i;
	uint32_t b, carry;
	int sourcePos      = 0; /* index of leftmost source uint8_t containing data (1 to 8 bits). */
	int sourceGap      = (8 - ((int)sourceBits & 7)) & 7; /* space on source[sourcePos]. */
	int bufferRem      = context->bufferBits & 7; /* occupied bits on buffer[bufferPos]. */
	uint8_t *buffer    = context->buffer;
	uint8_t *bitLength = context->bitLength;
	int bufferBits     = context->bufferBits;
	int bufferPos      = context->bufferPos;
	
	/* tally the length of the added data */
	uint64_t value = sourceBits;
	
	for (i = 31, carry = 0; i >= 0 && (carry != 0 || value != 0ULL); i--) {
		carry += bitLength[i] + ((uint32_t)value & 0xff);
		bitLength[i] = (uint8_t)carry;
		carry >>= 8;
		value >>= 8;
	}
	
	/* process data in chunks of 8 bits */
	while (sourceBits > 8) {
		/* take a byte from the source */
		b = ((source[sourcePos] << sourceGap) & 0xff) |
				((source[sourcePos + 1] & 0xff) >> (8 - sourceGap));
		
		/* process this byte */
		buffer[bufferPos++] |= (uint8_t)(b >> bufferRem);
		bufferBits += 8 - bufferRem; /* bufferBits = 8*bufferPos; */
		
		if (bufferBits == DIGESTBITS) {
			/* process data block */
			whirlpool_transform(context);
			
			/* reset buffer */
			bufferBits = bufferPos = 0;
		}
		
		buffer[bufferPos] = b << (8 - bufferRem);
		bufferBits += bufferRem;
		
		/* proceed to remaining data */
		sourceBits -= 8;
		sourcePos++;
	}
	
	/* now 0 <= sourceBits <= 8;
	 * furthermore, all data (if any is left) is in source[sourcePos].
	 */
	if (sourceBits > 0) {
		b = (source[sourcePos] << sourceGap) & 0xff; /* bits are left-justified on b. */
		
		/* process the remaining bits */
		buffer[bufferPos] |= b >> bufferRem;
	}
	
	else
		b = 0;
	
	/* all remaining data fits on buffer[bufferPos],
	 * and there still remains some space.
	 */
	if (bufferRem + sourceBits < 8) 
		bufferBits += sourceBits;
	
	else {
		/* buffer[bufferPos] is full */
		bufferPos++;
		bufferBits += 8 - bufferRem; /* bufferBits = 8*bufferPos; */
		sourceBits -= 8 - bufferRem;
		
		/* now 0 <= sourceBits < 8;
		 * furthermore, all data (if any is left) is in source[sourcePos].
		 */
		if (bufferBits == DIGESTBITS) {
			/* process data block */
			whirlpool_transform(context);
			
			/* reset buffer */
			bufferBits = bufferPos = 0;
		}
		
		buffer[bufferPos] = b << (8 - bufferRem);
		bufferBits += (int)sourceBits;
	}
	
	context->bufferBits = bufferBits;
	context->bufferPos  = bufferPos;
}
