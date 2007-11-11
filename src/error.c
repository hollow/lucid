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

#include <stdarg.h>
#include <string.h>

#include "error.h"
#include "printf.h"
#include "str.h"

error_t ___lucid_error = { { NULL, NULL }, NULL, NULL, 0, 0, NULL };
error_t *__lucid_error = &___lucid_error;

void error_init(error_t *err)
{
	INIT_LIST_HEAD(&(err->list));
}

void error_push(error_t *err, const char *file, int line, const char *func,
		int errnum, const char *fmt, ...)
{
	if (!err->list.next)
		error_init(err);

	error_t *new = mem_alloc(sizeof(error_t));

	new->file   = file;
	new->func   = func;
	new->line   = line;
	new->errnum = errnum;

	if (fmt == NULL)
		new->msg = NULL;

	else {
		char *buf;

		va_list ap;
		va_start(ap, fmt);
		_lucid_vasprintf(&buf, fmt, ap);
		va_end(ap);

		new->msg = buf;
	}

	list_add(&(new->list), &(err->list));
}

bool error_empty(error_t *err)
{
	if (!err->list.next)
		error_init(err);

	return list_empty(&(err->list));
}

error_t *error_pop(error_t *err)
{
	if (!err->list.next)
		error_init(err);

	error_t *top;
	list_t *pos;

	if (error_empty(err))
		return NULL;

	pos = err->list.next;
	top = list_entry(pos, error_t, list);

	list_del(pos);
	return top;
}

void error_free_node(error_t *err)
{
	if (err->msg) mem_free(err->msg);
	mem_free(err);
}

void error_free(error_t *err)
{
	if (!err->list.next)
		error_init(err);

	int errno_orig = errno;
	error_t *p;
	list_t *pos, *tmp;

	list_for_each_safe(pos, tmp, &(err->list)) {
		p = list_entry(pos, error_t, list);
		list_del(pos);
		error_free_node(p);
	}

	errno = errno_orig;
}

int error_count(error_t *err)
{
	if (!err->list.next)
		error_init(err);

	list_t *pos;
	int count = 0;

	list_for_each(pos, &(err->list))
		count++;

	return count;
}

char *error_describe(error_t *err, const char *prefix)
{
	char *buf    = NULL,
		 *errmsg = NULL,
		 *errstr = NULL,
		 *file   = str_path_basename(err->file);

	if (err->msg) {
		char *flat = str_flatten(err->msg);
		asprintf(&errmsg, "\n%10s%s", "", flat);
	}

	if (err->errnum > 0)
		asprintf(&errstr, "\n%10serrno = %d: %s", "",
				err->errnum, strerror(err->errnum));

	asprintf(&buf, "%s %s (%s:%d):%s%s",
			prefix, err->func, file, err->line,
			errmsg ? errmsg : "",
			errstr ? errstr : "");

	if (errmsg) mem_free(errmsg);
	if (errstr) mem_free(errstr);
	mem_free(file);

	return buf;
}

void error_print_trace(error_t *err, int fd)
{
	error_t *next = NULL, *cur = error_pop(err);
	assert(cur != NULL);

	dprintf(fd, "FATAL: caught runtime error\n");

	char *desc = error_describe(cur, "in  ");
	dprintf(fd, "%s\n", desc);
	mem_free(desc);
	error_free_node(cur);

	cur = error_pop(err);
	assert(cur != NULL);

	do {
		if (!(next = error_pop(err)))
			break;

		desc = error_describe(cur, "from");
		dprintf(fd, "%s\n", desc);
		mem_free(desc);
		error_free_node(cur);

		cur = next;
	} while (true);

	desc = error_describe(cur, "by  ");
	dprintf(fd, "%s\n", desc);
	mem_free(desc);
	error_free_node(cur);
}
