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
		new->msg = str_dup("no error message");

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

char *error_describe(error_t *err)
{
	char *buf,
		 *errnumstr = NULL,
		 *file = str_path_basename(err->file),
		 *msg  = str_flatten(err->msg);

	if (err->errnum > 0)
		asprintf(&errnumstr, "\n\terrno = %d: %s",
				err->errnum, strerror(err->errnum));

	asprintf(&buf, "in %s (%s:%d):\n\t%s%s",
			err->func,
			file,
			err->line,
			msg,
			errnumstr ? errnumstr : "");

	if (errnumstr) mem_free(errnumstr);
	mem_free(file);
	return buf;
}
