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
 * @defgroup flist Flag list conversion
 *
 * The flag list family of functions manages a list of possible values of a
 * bitmap using strings as key into the list.
 *
 * A bitmap is simply an integer with certain bits being 1 (enabled) and 0
 * (disabled).
 *
 * The FLIST32_START and FLIST64_START macros provides a shortcut for list
 * declaration and initialization; followed by one or more of FLIST32_NODE,
 * FLIST32_NODE1, FLIST64_NODE and FLIST64_NODE1 to insert nodes into the list.
 * The FLIST32_NODE1 and FLIST64_NODE1 macros are the same as FLIST32_NODE and
 * FLIST64_NODE, respectively, except that they convert the bit index to a
 * bitmap value before storing it in the list. The list should then be
 * terminated using FLIST32_END or FLIST64_END, respectively.
 *
 * The flist32_getval(), flist64_getval(), flist32_getkey() and flist64_getkey()
 * functions provide lookup routines by key and value, respectively.
 *
 * The flist32_from_str() and flist64_from_str() functions convert a string
 * consisting of zero or more flag list keys seperated by a delimiter and
 * optionally prefixed with a clear modifier to a bitmap/bitmask pair according
 * to a given list.
 *
 * The flist32_to_str() and flist64_to_str() functions convert a bitmap
 * according to a given list to a string consisting of zero or more flag list
 * keys seperated by a delimiter.
 *
 * @{
 */

#ifndef _LUCID_FLIST_H
#define _LUCID_FLIST_H

#include <stdint.h>

typedef struct flag32 {
	uint32_t flag;
	uint32_t mask;
} flag32_t;

/*! @brief 32 bit list object */
typedef struct flist32 {
	const char *key;    /*!< Node key (must be unique) */
	const uint32_t val; /*!< Node value (32-bit) */
} flist32_t;

/*! @brief 32 bit list initialization */
#define FLIST32_START(LIST) const flist32_t LIST[] = {

/*! @brief 32 bit list node */
#define FLIST32_NODE(PREFIX, NAME) { #NAME, PREFIX ## _ ## NAME },

/*! @brief 32 bit list node from index */
#define FLIST32_NODE1(PREFIX, NAME) { #NAME, (1 << PREFIX ## _ ## NAME) },

/*! @brief 32 bit list termination */
#define FLIST32_END { 0, 0 } };

/*!
 * @brief get 32 bit value by key
 *
 * @param[in] list list to use for conversion
 * @param[in] key  key to look for
 *
 * @return 32 bit value >= 1 if key found, 0 otherwise
 */
uint32_t flist32_getval(const flist32_t list[], const char *key);

/*!
 * @brief get key from 32 bit value
 *
 * @param[in] list list to use for conversion
 * @param[in] val  32 bit key to look for
 *
 * @return key if value was found, NULL otherwise
 *
 * @note this functions does not reset the flags or mask argument to an empty
 *       bitmap, thus allowing incremental changes to the map.
 */
const char *flist32_getkey(const flist32_t list[], uint32_t val);

/*!
 * @brief parse flag list string
 *
 * @param[in]  str   string to convert
 * @param[in]  list  list to use for conversion
 * @param[out] flags pointer to a bit mask
 * @param[out] mask  pointer to a set mask
 * @param[in]  clmod clear flag modifier
 * @param[in]  delim flag delimiter
 *
 * @return 0 on success, -1 on error with errno set
 */
int flist32_decode(const char *str, const flist32_t list[], flag32_t *flag32,
		char clmod, const char *delim);

/*!
 * @brief convert bit mask to flag list string
 *
 * @param[in] list  list to use for conversion
 * @param[in] val   bit mask
 * @param[in] delim flag delimiter
 *
 * @return flags list string
 *
 * @note this function ignores set bits if they do not appear in the list
 * @note if no flag was found or the bitmap was empty, an empty string is
 *       returned, not NULL
 */
char *flist32_encode(const flist32_t list[], const flag32_t *flag32,
		char clmod, const char *delim);


typedef struct {
	uint64_t flag;
	uint64_t mask;
} flag64_t;

/*! @brief 64 bit list object */
typedef struct {
	const char *key;    /*!< Node key (must be unique) */
	const uint64_t val; /*!< Node value (64-bit) */
} flist64_t;

/*! @brief 64 bit list initialization */
#define FLIST64_START(LIST) const flist64_t LIST[] = {

/*! @brief 64 bit list node */
#define FLIST64_NODE(PREFIX, NAME) { #NAME, PREFIX ## _ ## NAME },

/*! @brief 64 bit list node from index */
#define FLIST64_NODE1(PREFIX, NAME) { #NAME, (1 << PREFIX ## _ ## NAME) },

/*! @brief 64 bit list termination */
#define FLIST64_END { 0, 0 } };

/*!
 * @brief get 64 bit value by key
 *
 * @param[in] list list to use for conversion
 * @param[in] key  key to look for
 *
 * @return 64 bit value >= 1 if key was found, 0 otherwise
 */
uint64_t flist64_getval(const flist64_t list[], const char *key);

/*!
 * @brief get key from 64 bit value
 *
 * @param[in] list list to use for conversion
 * @param[in] val  64 bit key to look for
 *
 * @return key if value was found, NULL otherwise
 */
const char *flist64_getkey(const flist64_t list[], uint64_t val);

/*!
 * @brief parse flag list string
 *
 * @param[in]  str   string to convert
 * @param[in]  list  list to use for conversion
 * @param[out] flags pointer to a bit mask
 * @param[out] mask  pointer to a set mask
 * @param[in]  clmod clear flag modifier
 * @param[in]  delim flag delimiter
 *
 * @return 0 on success, -1 on error with errno set
 *
 * @note this functions does not reset the flags or mask argument to an empty
 *       bitmap, thus allowing incremental changes to the map.
 */
int flist64_decode(const char *str, const flist64_t list[], flag64_t *flag64,
		char clmod, const char *delim);

/*!
 * @brief convert bit mask to flag list string
 *
 * @param[in] list  list to use for conversion
 * @param[in] val   bit mask
 * @param[in] delim flag delimiter
 *
 * @return flags list string
 *
 * @note this function ignores set bits if they do not appear in the list
 * @note if no flag was found or the bitmap was empty, an empty string is
 *       returned, not NULL
 */
char *flist64_encode(const flist64_t list[], const flag64_t *flag64,
		char clmod, const char *delim);

#endif

/*! @} flist */
