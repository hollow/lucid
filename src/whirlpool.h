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

/*!
 * @defgroup whirlpool Whirlpool hash function
 *
 * WHIRLPOOL is a cryptographic hash function designed after the Square block
 * cipher. WHIRLPOOL is a Miyaguchi-Preneel construction based on a
 * substantially modified Advanced Encryption Standard (AES). Given a message
 * less than 2^256 bits in length, it returns a 512-bit message digest.
 *
 * The whirlpool_init() function initializes the hash context pointed to by
 * context. After initialization input can be added to the transform routine
 * using whirlpool_add(). Once all bytes have been added the transform has to be
 * finished by calling whilrpool_finalize().
 *
 * An application should not directly use the internal whirlpool_transform()
 * function, but always use whirlpool_add().
 *
 * The whirlpool_digest() function combines the procedure explained above for a
 * single string and returns the digest in hexadecimal notation.
 *
 * @{
 */

#ifndef _LUCID_WHIRLPOOL_H
#define _LUCID_WHIRLPOOL_H

#include <stdint.h>

/*! @brief number of bytes in the digest */
#define DIGESTBYTES 64

/*! @brief number of bits in the digest */
#define DIGESTBITS  (8*DIGESTBYTES) /* 512 */


/*! @brief number of bytes in the input buffer */
#define WBLOCKBYTES 64

/*! @brief number of bits in the input buffer */
#define WBLOCKBITS  (8*WBLOCKBYTES) /* 512 */


/*! @brief number of hashed bytes */
#define LENGTHBYTES 32

/*! @brief number of hashed bits */
#define LENGTHBITS  (8*LENGTHBYTES) /* 256 */

/*!
 * @brief dynamic whirlpool state data
 *
 * This struct is used to keep track of the whirlpool transform, i.e. its
 * hashing state, input buffer, number of hashed bits, etc.
 */
typedef struct {
	uint8_t  len[LENGTHBYTES];    /*!< global number of hashed bits */
	uint8_t  buf[WBLOCKBYTES];    /*!< buffer of data to hash */
	int      bits;                /*!< current number of bits on the buffer */
	int      pos;                 /*!< current (possibly incomplete) byte slot on the buffer */
	uint64_t hash[DIGESTBYTES/8]; /*!< the hashing state */
} whirlpool_t;

/*!
 * @brief internal transform routine
 *
 * @param[in] context whirlpool state context
 */
void whirlpool_transform(whirlpool_t * const context);

/*!
 * @brief initialize whirlpool state context
 *
 * @param[in] context whirlpool state context
 */
void whirlpool_init(whirlpool_t * const context);

/*!
 * @brief finalize whirlpool transformation
 *
 * @param[in]  context whirlpool state context
 * @param[out] result  string to store digest
 */
void whirlpool_finalize(whirlpool_t * const context, unsigned char * const result);

/*!
 * @brief add bytes to the transform routine
 *
 * @param[in] context whirlpool state context
 * @param[in] src     source string
 * @param[in] bits    number of bits in the source string
 */
void whirlpool_add(whirlpool_t * const context,
                   const unsigned char * const src, unsigned long bits);

/*!
 * @brief create digest from string
 *
 * @param[in] str source string
 *
 * @return digest string (memory obtained by malloc(3))
 *
 * @note The caller should free obtained memory using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 */
char *whirlpool_digest(const char *str);

#endif

/*! @} str */
