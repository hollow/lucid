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
 * @defgroup flist Flag list conversion
 * @{
 */

#ifndef _LUCID_FLIST_H
#define _LUCID_FLIST_H

#include <stdint.h>

/*!
 * @defgroup flist32 32-bit lists
 * @{
 */
/*! @brief 32 bit list object */
typedef struct {
	const char *key;
	const uint32_t val;
} flist32_t;

/*! @brief 32 bit list initialization */
#define FLIST32_START(LIST) const flist32_t LIST[] = {

/*! @brief 32 bit list node */
#define FLIST32_NODE(PREFIX, NAME) { #NAME, PREFIX ## _ ## NAME },

/*! @brief 32 bit list node from index */
#define FLIST32_NODE1(PREFIX, NAME) { #NAME, (1 << PREFIX ## _ ## NAME) },

/*! @brief 32 bit list termination */
#define FLIST32_END { NULL, 0 } };

/*!
 * @brief get 32 bit value by key
 *
 * @param list list to use for conversion
 * @param key  key to look for
 *
 * @return 32 bit value >= 1 if key found, 0 otherwise
 */
uint32_t flist32_getval(const flist32_t list[], const char *key);

/*!
 * @brief get key from 32 bit value
 *
 * @param list list to use for conversion
 * @param val  32 bit key to look for
 *
 * @return key if value was found, NULL otherwise
 */
const char *flist32_getkey(const flist32_t list[], uint32_t val);

/*!
 * @brief convert 32 bit value to bit index
 *
 * @param val 32 bit value
 *
 * @return bit index (0-31)
 */
int flist32_val2index(uint32_t val);

/*!
 * @brief convert bit index to 32 bit value
 *
 * @param index bit index (0-31)
 *
 * @return 32 bit value
 */
uint32_t    flist32_index2val(int index);

/*!
 * @brief parse flag list string
 *
 * @param str   string to convert
 * @param list  list to use for conversion
 * @param flags pointer to a bit mask
 * @param mask  pointer to a set mask
 * @param clmod clear flag modifier
 * @param delim flag delimiter
 *
 * @return 0 on success, -1 on error with errno set
 */
int flist32_parse(const char *str, const flist32_t list[],
                  uint32_t *flags, uint32_t *mask,
                  char clmod, char delim);

/*!
 * @brief convert bit mask to flag list string
 *
 * @param list  list to use for conversion
 * @param val   bit mask
 * @param delim flag delimiter
 *
 * @return flags list string (with zero length if no flag was found)
 *
 * @note this function ignores set bits if they do not appear in the list
 */
char *flist32_tostr(const flist32_t list[], uint32_t val, char delim);
/*! @} flist32 */



/*!
 * @defgroup flist64 64-bit lists
 * @{
 */
/*! @brief 64 bit list object */
typedef struct {
	const char *key;
	const uint64_t val;
} flist64_t;

/*! @brief 64 bit list initialization */
#define FLIST64_START(LIST) const flist64_t LIST[] = {

/*! @brief 64 bit list node */
#define FLIST64_NODE(PREFIX, NAME) { #NAME, PREFIX ## _ ## NAME },

/*! @brief 64 bit list node from index */
#define FLIST64_NODE1(PREFIX, NAME) { #NAME, (1 << PREFIX ## _ ## NAME) },

/*! @brief 64 bit list termination */
#define FLIST64_END { NULL, 0 } };

/*!
 * @brief get 64 bit value by key
 *
 * @param list list to use for conversion
 * @param key  key to look for
 *
 * @return 64 bit value >= 1 if key was found, 0 otherwise
 */
uint64_t flist64_getval(const flist64_t list[], const char *key);

/*!
 * @brief get key from 64 bit value
 *
 * @param list list to use for conversion
 * @param val  64 bit key to look for
 *
 * @return key if value was found, NULL otherwise
 */
const char *flist64_getkey(const flist64_t list[], uint64_t val);

/*!
 * @brief convert 64 bit value to bit index
 *
 * @param val 64 bit value
 *
 * @return bit index (0-63)
 */
int flist64_val2index(uint64_t val);

/*!
 * @brief convert bit index to 64 bit value
 *
 * @param index bit index (0-63)
 *
 * @return 64 bit value
 */
uint64_t flist64_index2val(int index);

/*!
 * @brief parse flag list string
 *
 * @param str   string to convert
 * @param list  list to use for conversion
 * @param flags pointer to a bit mask
 * @param mask  pointer to a set mask
 * @param clmod clear flag modifier
 * @param delim flag delimiter
 *
 * @return 0 on success, -1 on error with errno set
 */
int flist64_parse(const char *str, const flist64_t list[],
                  uint64_t *flags, uint64_t *mask,
                  char clmod, char delim);

/*!
 * @brief convert bit mask to flag list string
 *
 * @param list  list to use for conversion
 * @param val   bit mask
 * @param delim flag delimiter
 *
 * @return flags list string (with zero length if no flag was found)
 *
 * @note this function ignores set bits if they do not appear in the list
 */
char *flist64_tostr(const flist64_t list[], uint64_t val, char delim);
/*! @} flist64 */

#endif

/*! @} flist */
