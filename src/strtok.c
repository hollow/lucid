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

#include <string.h>
#include <errno.h>

#include "cext.h"
#include "str.h"
#include "stralloc.h"
#include "strtok.h"

strtok_t *strtok_init_argv(strtok_t *st, char *argv[], int argc, int empty)
{
	int i;
	strtok_t *new;

	INIT_LIST_HEAD(&(st->list));

	for (i = 0; i < argc; i++) {
		if (!empty && str_isempty(argv[i]))
			continue;

		if (!(new = malloc(sizeof(strtok_t)))) {
			strtok_free(st);
			return 0;
		}

		if (!(new->token = str_dup(argv[i]))) {
			strtok_free(st);
			return 0;
		}

		list_add_tail(&(new->list), &(st->list));
	}

	return st;
}

strtok_t *strtok_init_str(strtok_t *st, const char *str, const char *delim, int empty)
{
	strtok_t *new;
	char *scpy, *cur, *token;

	INIT_LIST_HEAD(&(st->list));

	if (!str)
		return st;

	scpy = cur = token = str_dup(str);

	if (!scpy)
		return 0;

	while (token) {
		cur = str_str(cur, delim);

		if (cur) {
			memset(cur, 0, str_len(delim));
			cur += str_len(delim);
		}

		if (empty || !str_isempty(token)) {
			if (!(new = malloc(sizeof(strtok_t))))
				goto free;

			if (!(new->token = str_dup(token)))
				goto free;

			list_add_tail(&(new->list), &(st->list));
		}

		token = cur;
	}

	goto out;

free:
	strtok_free(st);
	st = 0;

out:
	free(scpy);
	return st;
}

void strtok_free(strtok_t *st)
{
	int errno_orig = errno;
	strtok_t *p;
	list_t *pos, *tmp;

	list_for_each_safe(pos, tmp, &(st->list)) {
		p = list_entry(pos, strtok_t, list);
		list_del(pos);

		if (p->token)
			free(p->token);

		free(p);
	}

	errno = errno_orig;
}

int strtok_count(strtok_t *st)
{
	list_t *pos;
	int count = 0;

	list_for_each(pos, &(st->list))
		count++;

	return count;
}

int strtok_append(strtok_t *st, const char *token)
{
	strtok_t *new;

	if (!(new = malloc(sizeof(strtok_t))))
		return -1;

	if (!(new->token = str_dup(token))) {
		free(new);
		return -1;
	}

	list_add_tail(&(new->list), &(st->list));

	return 0;
}

void strtok_delete(strtok_t *st, const char *token)
{
	strtok_t *p;
	list_t *pos, *tmp;

	list_for_each_safe(pos, tmp, &(st->list)) {
		p = list_entry(pos, strtok_t, list);

		if (str_equal(p->token, token)) {
			list_del(pos);
			free(p->token);
			free(p);
		}
	}
}

char *strtok_prev(strtok_t **st)
{
	strtok_t *oldhead = *st;
	list_t   *prev    = (&(oldhead->list))->prev;
	strtok_t *newhead = list_entry(prev, strtok_t, list);

	*st = newhead;

	return oldhead->token;
}

char *strtok_next(strtok_t **st)
	{
	strtok_t *oldhead = *st;
	list_t   *next    = (&(oldhead->list))->next;
	strtok_t *newhead = list_entry(next, strtok_t, list);

	*st = newhead;

	return oldhead->token;
}

int strtok_toargv(strtok_t *st, char **argv)
{
	int i = 0;
	strtok_t *p;

	if (strtok_count(st) < 1)
		return 0;

	strtok_for_each(st, p)
		argv[i++] = p->token;

	argv[i] = NULL;

	return i;
}

int strtok_tostr(strtok_t *st, char **str, char *delim)
{
	int i = 0;
	stralloc_t _sa, *sa = &_sa;
	strtok_t *p;

	if (strtok_count(st) < 1)
		return 0;

	stralloc_init(sa);

	strtok_for_each(st, p) {
		if (stralloc_catf(sa, "%s%s", p->token, delim) == -1)
			return -1;

		i++;
	}

	if (sa->len > 0)
		sa->len -= str_len(delim);

	*str = stralloc_finalize(sa);

	stralloc_free(sa);
	return i;
}
