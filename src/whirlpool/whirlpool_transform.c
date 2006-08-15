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

#include <stdint.h>

#include "whirlpool.h"
#include "whirlpool_tables.h"

void whirlpool_transform(whirlpool_t * const context)
{
	int i, r;
	uint64_t K[8];
	uint64_t block[8];
	uint64_t state[8];
	uint64_t L[8];
	uint8_t *buffer = context->buffer;

	/* map the buffer to a block */
	for (i = 0; i < 8; i++, buffer += 8) {
		block[i] = (((uint64_t)buffer[0]        ) << 56) ^
		           (((uint64_t)buffer[1] & 0xffL) << 48) ^
		           (((uint64_t)buffer[2] & 0xffL) << 40) ^
		           (((uint64_t)buffer[3] & 0xffL) << 32) ^
		           (((uint64_t)buffer[4] & 0xffL) << 24) ^
		           (((uint64_t)buffer[5] & 0xffL) << 16) ^
		           (((uint64_t)buffer[6] & 0xffL) <<  8) ^
		           (((uint64_t)buffer[7] & 0xffL)      );
	}
	
	/* compute and apply K^0 to the cipher state */
	state[0] = block[0] ^ (K[0] = context->hash[0]);
	state[1] = block[1] ^ (K[1] = context->hash[1]);
	state[2] = block[2] ^ (K[2] = context->hash[2]);
	state[3] = block[3] ^ (K[3] = context->hash[3]);
	state[4] = block[4] ^ (K[4] = context->hash[4]);
	state[5] = block[5] ^ (K[5] = context->hash[5]);
	state[6] = block[6] ^ (K[6] = context->hash[6]);
	state[7] = block[7] ^ (K[7] = context->hash[7]);
	
	/* iterate over all rounds */
	for (r = 1; r <= R; r++) {
		/* compute K^r from K^{r-1} */
		L[0] = C0[(int)(K[0] >> 56)       ] ^
		       C1[(int)(K[7] >> 48) & 0xff] ^
		       C2[(int)(K[6] >> 40) & 0xff] ^
		       C3[(int)(K[5] >> 32) & 0xff] ^
		       C4[(int)(K[4] >> 24) & 0xff] ^
		       C5[(int)(K[3] >> 16) & 0xff] ^
		       C6[(int)(K[2] >>  8) & 0xff] ^
		       C7[(int)(K[1]      ) & 0xff] ^
		       rc[r];
		
		L[1] = C0[(int)(K[1] >> 56)       ] ^
		       C1[(int)(K[0] >> 48) & 0xff] ^
		       C2[(int)(K[7] >> 40) & 0xff] ^
		       C3[(int)(K[6] >> 32) & 0xff] ^
		       C4[(int)(K[5] >> 24) & 0xff] ^
		       C5[(int)(K[4] >> 16) & 0xff] ^
		       C6[(int)(K[3] >>  8) & 0xff] ^
		       C7[(int)(K[2]      ) & 0xff];
		
		L[2] = C0[(int)(K[2] >> 56)       ] ^
		       C1[(int)(K[1] >> 48) & 0xff] ^
		       C2[(int)(K[0] >> 40) & 0xff] ^
		       C3[(int)(K[7] >> 32) & 0xff] ^
		       C4[(int)(K[6] >> 24) & 0xff] ^
		       C5[(int)(K[5] >> 16) & 0xff] ^
		       C6[(int)(K[4] >>  8) & 0xff] ^
		       C7[(int)(K[3]      ) & 0xff];
		
		L[3] = C0[(int)(K[3] >> 56)       ] ^
		       C1[(int)(K[2] >> 48) & 0xff] ^
		       C2[(int)(K[1] >> 40) & 0xff] ^
		       C3[(int)(K[0] >> 32) & 0xff] ^
		       C4[(int)(K[7] >> 24) & 0xff] ^
		       C5[(int)(K[6] >> 16) & 0xff] ^
		       C6[(int)(K[5] >>  8) & 0xff] ^
		       C7[(int)(K[4]      ) & 0xff];
		
		L[4] = C0[(int)(K[4] >> 56)       ] ^
		       C1[(int)(K[3] >> 48) & 0xff] ^
		       C2[(int)(K[2] >> 40) & 0xff] ^
		       C3[(int)(K[1] >> 32) & 0xff] ^
		       C4[(int)(K[0] >> 24) & 0xff] ^
		       C5[(int)(K[7] >> 16) & 0xff] ^
		       C6[(int)(K[6] >>  8) & 0xff] ^
		       C7[(int)(K[5]      ) & 0xff];
		
		L[5] = C0[(int)(K[5] >> 56)       ] ^
		       C1[(int)(K[4] >> 48) & 0xff] ^
		       C2[(int)(K[3] >> 40) & 0xff] ^
		       C3[(int)(K[2] >> 32) & 0xff] ^
		       C4[(int)(K[1] >> 24) & 0xff] ^
		       C5[(int)(K[0] >> 16) & 0xff] ^
		       C6[(int)(K[7] >>  8) & 0xff] ^
		       C7[(int)(K[6]      ) & 0xff];
		
		L[6] = C0[(int)(K[6] >> 56)       ] ^
		       C1[(int)(K[5] >> 48) & 0xff] ^
		       C2[(int)(K[4] >> 40) & 0xff] ^
		       C3[(int)(K[3] >> 32) & 0xff] ^
		       C4[(int)(K[2] >> 24) & 0xff] ^
		       C5[(int)(K[1] >> 16) & 0xff] ^
		       C6[(int)(K[0] >>  8) & 0xff] ^
		       C7[(int)(K[7]      ) & 0xff];
		
		L[7] = C0[(int)(K[7] >> 56)       ] ^
		       C1[(int)(K[6] >> 48) & 0xff] ^
		       C2[(int)(K[5] >> 40) & 0xff] ^
		       C3[(int)(K[4] >> 32) & 0xff] ^
		       C4[(int)(K[3] >> 24) & 0xff] ^
		       C5[(int)(K[2] >> 16) & 0xff] ^
		       C6[(int)(K[1] >>  8) & 0xff] ^
		       C7[(int)(K[0]      ) & 0xff];
		
		K[0] = L[0];
		K[1] = L[1];
		K[2] = L[2];
		K[3] = L[3];
		K[4] = L[4];
		K[5] = L[5];
		K[6] = L[6];
		K[7] = L[7];
		
		/* apply the r-th round transformation */
		L[0] = C0[(int)(state[0] >> 56)       ] ^
		       C1[(int)(state[7] >> 48) & 0xff] ^
		       C2[(int)(state[6] >> 40) & 0xff] ^
		       C3[(int)(state[5] >> 32) & 0xff] ^
		       C4[(int)(state[4] >> 24) & 0xff] ^
		       C5[(int)(state[3] >> 16) & 0xff] ^
		       C6[(int)(state[2] >>  8) & 0xff] ^
		       C7[(int)(state[1]      ) & 0xff] ^
		       K[0];
		
		L[1] = C0[(int)(state[1] >> 56)       ] ^
		       C1[(int)(state[0] >> 48) & 0xff] ^
		       C2[(int)(state[7] >> 40) & 0xff] ^
		       C3[(int)(state[6] >> 32) & 0xff] ^
		       C4[(int)(state[5] >> 24) & 0xff] ^
		       C5[(int)(state[4] >> 16) & 0xff] ^
		       C6[(int)(state[3] >>  8) & 0xff] ^
		       C7[(int)(state[2]      ) & 0xff] ^
		       K[1];
		
		L[2] = C0[(int)(state[2] >> 56)       ] ^
		       C1[(int)(state[1] >> 48) & 0xff] ^
		       C2[(int)(state[0] >> 40) & 0xff] ^
		       C3[(int)(state[7] >> 32) & 0xff] ^
		       C4[(int)(state[6] >> 24) & 0xff] ^
		       C5[(int)(state[5] >> 16) & 0xff] ^
		       C6[(int)(state[4] >>  8) & 0xff] ^
		       C7[(int)(state[3]      ) & 0xff] ^
		       K[2];
		
		L[3] = C0[(int)(state[3] >> 56)       ] ^
		       C1[(int)(state[2] >> 48) & 0xff] ^
		       C2[(int)(state[1] >> 40) & 0xff] ^
		       C3[(int)(state[0] >> 32) & 0xff] ^
		       C4[(int)(state[7] >> 24) & 0xff] ^
		       C5[(int)(state[6] >> 16) & 0xff] ^
		       C6[(int)(state[5] >>  8) & 0xff] ^
		       C7[(int)(state[4]      ) & 0xff] ^
		       K[3];
		
		L[4] = C0[(int)(state[4] >> 56)       ] ^
		       C1[(int)(state[3] >> 48) & 0xff] ^
		       C2[(int)(state[2] >> 40) & 0xff] ^
		       C3[(int)(state[1] >> 32) & 0xff] ^
		       C4[(int)(state[0] >> 24) & 0xff] ^
		       C5[(int)(state[7] >> 16) & 0xff] ^
		       C6[(int)(state[6] >>  8) & 0xff] ^
		       C7[(int)(state[5]      ) & 0xff] ^
		       K[4];
		
		L[5] = C0[(int)(state[5] >> 56)       ] ^
		       C1[(int)(state[4] >> 48) & 0xff] ^
		       C2[(int)(state[3] >> 40) & 0xff] ^
		       C3[(int)(state[2] >> 32) & 0xff] ^
		       C4[(int)(state[1] >> 24) & 0xff] ^
		       C5[(int)(state[0] >> 16) & 0xff] ^
		       C6[(int)(state[7] >>  8) & 0xff] ^
		       C7[(int)(state[6]      ) & 0xff] ^
		       K[5];
		
		L[6] = C0[(int)(state[6] >> 56)       ] ^
		       C1[(int)(state[5] >> 48) & 0xff] ^
		       C2[(int)(state[4] >> 40) & 0xff] ^
		       C3[(int)(state[3] >> 32) & 0xff] ^
		       C4[(int)(state[2] >> 24) & 0xff] ^
		       C5[(int)(state[1] >> 16) & 0xff] ^
		       C6[(int)(state[0] >>  8) & 0xff] ^
		       C7[(int)(state[7]      ) & 0xff] ^
		       K[6];
		
		L[7] = C0[(int)(state[7] >> 56)       ] ^
		       C1[(int)(state[6] >> 48) & 0xff] ^
		       C2[(int)(state[5] >> 40) & 0xff] ^
		       C3[(int)(state[4] >> 32) & 0xff] ^
		       C4[(int)(state[3] >> 24) & 0xff] ^
		       C5[(int)(state[2] >> 16) & 0xff] ^
		       C6[(int)(state[1] >>  8) & 0xff] ^
		       C7[(int)(state[0]      ) & 0xff] ^
		       K[7];
		
		state[0] = L[0];
		state[1] = L[1];
		state[2] = L[2];
		state[3] = L[3];
		state[4] = L[4];
		state[5] = L[5];
		state[6] = L[6];
		state[7] = L[7];
	}
	
	/* apply the Miyaguchi-Preneel compression function */
	context->hash[0] ^= state[0] ^ block[0];
	context->hash[1] ^= state[1] ^ block[1];
	context->hash[2] ^= state[2] ^ block[2];
	context->hash[3] ^= state[3] ^ block[3];
	context->hash[4] ^= state[4] ^ block[4];
	context->hash[5] ^= state[5] ^ block[5];
	context->hash[6] ^= state[6] ^ block[6];
	context->hash[7] ^= state[7] ^ block[7];
}
