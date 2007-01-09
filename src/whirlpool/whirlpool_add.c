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

#include "whirlpool.h"

void whirlpool_add(whirlpool_t * const context,
                   const unsigned char * const src, unsigned long srcbits)
{
	int i;
	uint32_t b, carry;
	int srcpos = 0; /* index of leftmost source uint8_t containing data (1 to 8 bits). */
	
	int gap      = (8 - ((int)srcbits & 7)) & 7; /* space on src[srcpos]. */
	int rem      = context->bits & 7; /* occupied bits on buf[pos]. */
	
	uint8_t *buf = context->buf;
	uint8_t *len = context->len;
	int bits     = context->bits;
	int pos      = context->pos;
	
	/* tally the length of the added data */
	uint64_t value = srcbits;
	
	for (i = 31, carry = 0; i >= 0 && (carry != 0 || value != 0ULL); i--) {
		carry += len[i] + ((uint32_t)value & 0xff);
		len[i] = (uint8_t)carry;
		carry >>= 8;
		value >>= 8;
	}
	
	/* process data in chunks of 8 bits */
	while (srcbits > 8) {
		/* take a byte from the source */
		b = ((src[srcpos] << gap) & 0xff) |
				((src[srcpos + 1] & 0xff) >> (8 - gap));
		
		/* process this byte */
		buf[pos++] |= (uint8_t)(b >> rem);
		bits += 8 - rem; /* bits = 8*pos; */
		
		if (bits == DIGESTBITS) {
			/* process data block */
			whirlpool_transform(context);
			
			/* reset buf */
			bits = pos = 0;
		}
		
		buf[pos] = b << (8 - rem);
		bits += rem;
		
		/* proceed to remaining data */
		srcbits -= 8;
		srcpos++;
	}
	
	/* now 0 <= srcbits <= 8;
	 * furthermore, all data (if any is left) is in src[srcpos].
	 */
	if (srcbits > 0) {
		b = (src[srcpos] << gap) & 0xff; /* bits are left-justified on b. */
		
		/* process the remaining bits */
		buf[pos] |= b >> rem;
	}
	
	else
		b = 0;
	
	/* all remaining data fits on buf[pos],
	 * and there still remains some space.
	 */
	if (rem + srcbits < 8) 
		bits += srcbits;
	
	else {
		/* buf[pos] is full */
		pos++;
		bits += 8 - rem; /* bits = 8*pos; */
		srcbits -= 8 - rem;
		
		/* now 0 <= srcbits < 8;
		 * furthermore, all data (if any is left) is in src[srcpos].
		 */
		if (bits == DIGESTBITS) {
			/* process data block */
			whirlpool_transform(context);
			
			/* reset buf */
			bits = pos = 0;
		}
		
		buf[pos] = b << (8 - rem);
		bits += (int)srcbits;
	}
	
	context->bits = bits;
	context->pos  = pos;
}
