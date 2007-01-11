// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
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

/*!
 * @defgroup mem Memory area manipulation
 *
 * @{
 */

#ifndef _LUCID_MEM_H
#define _LUCID_MEM_H

/*!
 * @brief allocate memory
 *
 * @param[in]  n allocate n bytes
 *
 * @return A pointer to newly allocated memory, NULL otherwise.
 */
void *mem_alloc(int n);

/*!
 * @brief copy memory block until character is found
 *
 * @param[out] s1 pointer to destination block
 * @param[in]  s2 pointer to source block
 * @param[in]  n  copy first n bytes of s2
 *
 * @return A pointer to s1.
 */
void *mem_ccpy(void *s1, const void *s2, int c, int n);

/*!
 * @brief find character in memory block
 *
 * @param[in] s pointer to memory block
 * @param[in] c character to look for
 * @param[in] n scan first n bytes
 *
 * @return A pointer to the first matching character found, NULL otherwise.
 */
void *mem_chr(const void *s, int c, int n);

/*!
 * @brief compare two memory regions
 *
 * @param[in] s1 pointer to first memory region
 * @param[in] s2 pointer to second memory region
 * @param[in] n  compare n bytes
 *
 * @return An integer less than, equal to, or greater than zero according to
 *         whether s1 is lexicographically less than, equal to, or greater
 *         than s2.
 */
int mem_cmp(const void *s1, const void *s2, int n);

/*!
 * @brief copy memory block
 *
 * @param[out] s1 pointer to destination block
 * @param[in]  s2 pointer to source block
 * @param[in]  n  copy first n bytes of s2
 *
 * @return A pointer to s1.
 */
void *mem_cpy(void *s1, const void *s2, int n);

/*!
 * @brief duplicate a memory block
 *
 * @param[in] s pointer to source memory area
 * @param[in] n duplicate first n bytes
 *
 * @return A pointer to the duplicated memory block, or NULL if insufficient
 *         memory was available.
 */
void *mem_dup(const void *s, int n);

/*!
 * @brief free memory
 *
 * @param[in]  s memory area to free
 */
void mem_free(void *s);

/*!
 * @brief find character in memory block
 *
 * @param[out] s pointer to memory block
 * @param[in]  c character to look for
 * @param[in]  n scan first n bytes
 *
 * @return An integer offset to the character in the memory block starting at s.
 *
 * @note This function will scan any number of bytes until a NULL character is
 *       found if n is less than or equal to zero.
 */
int mem_idx(const void *s, int c, int n);

/*!
 * @brief reallocate memory
 *
 * @param[in]  s memory area to reallocate
 * @param[in]  n allocate n bytes
 *
 * @return A pointer to newly allocated memory, NULL otherwise.
 */
void *mem_realloc(void *s, int n);

/*!
 * @brief fill memory block with character
 *
 * @param[out] s pointer to memory block
 * @param[in]  c character value to be set
 * @param[in]  n fill first n bytes
 *
 * @return A pointer to s1.
 */
void *mem_set(void *s, int c, int n);

#endif

/*! @} mem */
