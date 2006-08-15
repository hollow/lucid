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

#ifndef _LUCID_WHIRLPOOL_H
#define _LUCID_WHIRLPOOL_H

#include <stdint.h>

#define DIGESTBYTES 64
#define DIGESTBITS  (8*DIGESTBYTES) /* 512 */

#define WBLOCKBYTES 64
#define WBLOCKBITS  (8*WBLOCKBYTES) /* 512 */

#define LENGTHBYTES 32
#define LENGTHBITS  (8*LENGTHBYTES) /* 256 */

typedef struct {
	uint8_t  bitLength[LENGTHBYTES]; /* global number of hashed bits (256-bit counter) */
	uint8_t  buffer[WBLOCKBYTES];    /* buffer of data to hash */
	int      bufferBits;             /* current number of bits on the buffer */
	int      bufferPos;              /* current (possibly incomplete) byte slot on the buffer */
	uint64_t hash[DIGESTBYTES/8];    /* the hashing state */
} whirlpool_t;

void whirlpool_transform(whirlpool_t * const context);
void whirlpool_init(whirlpool_t * const context);
void whirlpool_finalize(whirlpool_t * const context, unsigned char * const result);
void whirlpool_add(whirlpool_t * const context,
                   const unsigned char * const source, unsigned long sourceBits);
char *whirlpool_digest(const char *str);

#endif
