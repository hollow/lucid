// Copyright 2005 Felix von Leitner <felix-libowfat@fefe.de>
//           2006 Benedikt Böhm <hollow@gentoo.org>
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
 * @defgroup stralloc Dynamic string allocator
 */
 
#ifndef _LUCID_STRALLOC_H
#define _LUCID_STRALLOC_H

#include <sys/types.h>

/*!
 * @brief dynamic string allocator tracking data
 *
 * This strcut is used to keep track of the dynamic string state, i.e. its
 * contents, its length and its additionaly allocated memory.
 */
typedef struct {
	char *s;    /*!< pointer to dynamic string */
	size_t len; /*!< current length of s */
	size_t a;   /*!< additional free memory in s */
} stralloc_t;

/*!
 * @brief initialize dynamic string allocator
 *
 * @param[out] sa string to initialize
 */
void stralloc_init(stralloc_t *sa);

/*!
 * @brief truncate string length to zero
 *
 * @param[out] sa string to truncate
 */
void stralloc_zero(stralloc_t *sa);

/*!
 * @brief ensure that enough memory has been allocated
 *
 * @param[in] sa  string to check
 * @param[in] len minimum length that has to be available
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_ready(stralloc_t *sa, size_t len);

/*!
 * @brief ensure that enough memory has been allocated
 *
 * @param[in] sa  string to check
 * @param[in] len additional length that has to be available
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_readyplus(stralloc_t *sa, size_t len);

/*!
 * @brief deallocate all memory
 *
 * @param[out] sa string to initialize
 */
void stralloc_free(stralloc_t *sa);


/*!
 * @brief copy a static string to a dynamic one
 *
 * @param[out] dst dynamic destination string
 * @param[in]  src static source string
 * @param[in]  len copy at most len bytes
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_copyb(stralloc_t *dst, const char *src, size_t len);

/*!
 * @brief copy a static string to a dynamic one
 *
 * @param[out] dst dynamic destination string
 * @param[in]  src static source string
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_copys(stralloc_t *dst, const char *src);

/*!
 * @brief copy one dynamic string to another
 *
 * @param[out] dst dynamic destination string
 * @param[in]  src dynamic source string
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_copy(stralloc_t *dst, const stralloc_t *src);


/*!
 * @brief concatenate a dynamic string and a static one
 *
 * @param[out] dst dynamic destination string
 * @param[in]  src static source string
 * @param[in]  len append at most len bytes
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_catb     (stralloc_t *dst, const char *src, size_t len);

/*!
 * @brief concatenate a dynamic string and a static one using formatted conversion
 *
 * @param[out] dst dynamic destination string
 * @param[in]  fmt format string
 * @param[in]  ... variable number of arguments
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_catf(stralloc_t *dst, const char *fmt, ...);

/*!
 * @brief concatenate a dynamic string and multiple static ones
 *
 * @param[out] dst dynamic destination string
 * @param[in]  ... variable number of source strings
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @note the last argument must be NULL
 */
int stralloc_catm(stralloc_t *dst, ...);

/*!
 * @brief concatenate a dynamic string and a static one
 *
 * @param[out] dst dynamic destination string
 * @param[in]  src static source string
 *
 * @return 0 on success, -1 on error with errno set
 */
int stralloc_cats(stralloc_t *dst, const char *src);

/*!
 * @brief concatenate two dynamic strings
 *
 * @param[out] dst dynamic destination string
 * @param[in]  src dynamic source string
 *
 * @return 0 on success, -1 on error with errno set
 */
int  stralloc_cat(stralloc_t *dst, const stralloc_t *src);


/*!
 * @brief compare two dynamic strings
 *
 * @param[in] a first string
 * @param[in] b second string
 *
 * @return An integer less than, equal to, or greater than zero if a is found,
 *         respectively, to be less than, to match, or be greater than b.
 */
int  stralloc_cmp      (const stralloc_t *a, const stralloc_t *b);

#endif

/*! @} stralloc */
