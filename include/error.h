// Copyright (C) 2007 Benedikt Böhm <hollow@gentoo.org>
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
 * @defgroup error Advanced error handling
 * @{
 */

#ifndef _LUCID_ERROR_H
#define _LUCID_ERROR_H

#define __error_t_defined
#include <errno.h>
#include <stdbool.h>

#undef NDEBUG
#include <assert.h>

#ifdef _LUCID_BUILD_
#include "list.h"
#else
#include <lucid/list.h>
#endif

/* assert stuff */
#define assert_not_reached() assert(true)

typedef struct error_s {
	list_t list;
	const char *file;
	const char *func;
	int line;
	int errnum;
	char *msg;
} error_t;

void error_init(error_t *err);

bool error_empty(error_t *err);

void error_push(error_t *err, const char *file, int line, const char *func,
		int errnum, const char *fmt, ...);

error_t *error_pop(error_t *err);

void error_free_node(error_t *err);

void error_free(error_t *err);

int error_count(error_t *err);

char *error_describe(error_t *err, const char *prefix);

void error_print_trace(error_t *err, int fd);

/* global error handling */
extern error_t *__lucid_error;

#define error_set(E, ...) do { \
	error_push(__lucid_error, __FILE__, __LINE__, __FUNCTION__, E, __VA_ARGS__); \
} while (0)

#define error_pass(...) error_set(errno, __VA_ARGS__)

#define error_dof(...) \
	if (!error_empty(__lucid_error)) { error_pass(__VA_ARGS__); } \
	if (!error_empty(__lucid_error))

#define error_do error_dof(NULL)

#define error_dump(fd) error_print_trace(__lucid_error, fd)

#define error_clear() error_free(__lucid_error)

#endif

/*! @} error */
