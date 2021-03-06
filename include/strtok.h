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
 * @defgroup strtok String tokenizer
 *
 * @{
 */

#ifndef _LUCID_STRTOK_H
#define _LUCID_STRTOK_H

#ifdef _LUCID_BUILD_
#include "list.h"
#else
#include <lucid/list.h>
#endif

typedef struct {
	list_t list;
	char *token;
} strtok_t;


/*!
 * @brief initialize string tokenizer from argument vector
 *
 * @param[out] st    tokenizer to initialize
 * @param[in]  argv  argument vector
 * @param[in]  argc  argument vector size
 * @param[in]  empty convert empty tokens
 *
 * @return A pointer to st.
 */
strtok_t *strtok_init_argv(strtok_t *st, char *argv[], int argc, int empty);

/*!
 * @brief initialize string tokenizer from character array
 *
 * @param[out] st    tokenizer to initialize
 * @param[in]  str   pointer to a string
 * @param[in]  delim token delimiter
 * @param[in]  empty convert empty tokens
 *
 * @return A pointer to st.
 */
strtok_t *strtok_init_str(strtok_t *st, const char *str, const char *delim, int empty);

/*!
 * @brief deallocate string tokenizer
 *
 * @param[out] st    tokenizer to free
 */
void strtok_free(strtok_t *st);


/*!
 * @brief count number of tokens
 *
 * @param[out] st    tokenizer to initialize
 *
 * @return Number of tokens in st.
 */
int strtok_count(strtok_t *st);


/*!
 * @brief append a token
 *
 * @param[out] st    tokenizer to append to
 * @param[in]  token token to append
 *
 * @return 0 on success, -1 on error with errno set.
 */
int strtok_append(strtok_t *st, const char *token);

/*!
 * @brief delete one or more tokens
 *
 * @param[out] st    tokenizer to delete from
 * @param[in]  token token to delete
 */
void strtok_delete(strtok_t *st, const char *token);

/*!
 * @brief Go to the previous token
 *
 * @param[in] st tokenizer to iterate
 */
char *strtok_prev(strtok_t **st);

/*!
 * @brief Go to the previous token
 *
 * @param[in] st tokenizer to iterate
 */
char *strtok_next(strtok_t **st);

/*! @brief interate through tokens */
#define strtok_for_each(st, p) list_for_each_entry(p, &(st->list), list)


/*!
 * @brief convert string tokenizer to argument vector
 *
 * @param[out] st   tokenizer to convert
 * @param[in]  argv argument vector big enough to hold all tokens
 *
 * @param 0 on success, -1 on error with errno set.
 */
int strtok_toargv(strtok_t *st, char **argv);

/*!
 * @brief convert string tokenizer to character array
 *
 * @param[out] st     tokenizer to convert
 * @param[in]  str    pointer to a string
 * @param[in]  delim  token delimiter
 *
 * @param 0 on success, -1 on error with errno set.
 */
int strtok_tostr(strtok_t *st, char **str, char *delim);

#endif

/*! @} strtok */
