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

#include "flist.h"
#include "str.h"
#include "stralloc.h"
#include "strtok.h"

const char *flist32_getkey(const flist32_t list[], uint32_t val)
{
	int i;

	for (i = 0; list[i].key; i++)
		if (list[i].val == val)
			return list[i].key;

	return 0;
}

uint32_t flist32_getval(const flist32_t list[], const char *key)
{
	int i;

	for (i = 0; list[i].key; i++)
		if (str_equal(list[i].key, key))
			return list[i].val;

	return 0;
}

int flist32_from_str(const char *str, const flist32_t list[],
		uint32_t *flags, uint32_t *mask,
		char clmod, const char *delim)
{
	char *token;
	int clear = 0;
	uint32_t cur_flag;

	strtok_t _st, *st = &_st, *p;

	if (!strtok_init_str(st, str, delim, 0))
		return -1;

	strtok_for_each(st, p) {
		token = p->token;

		if (*token == clmod)
			clear = 1;

		cur_flag = flist32_getval(list, token+clear);

		if (!cur_flag) {
			strtok_free(st);
			return -1;
		}

		if (clear) {
			*flags &= ~cur_flag;
			*mask  |=  cur_flag;
		} else {
			*flags |=  cur_flag;
			*mask  |=  cur_flag;
		}
	}

	strtok_free(st);

	return 0;
}

char *flist32_to_str(const flist32_t list[], uint32_t val, const char *delim)
{
	int i;
	char *buf;
	stralloc_t _sa, *sa = &_sa;

	stralloc_init(sa);

	for (i = 0; list[i].key; i++)
		if (val & list[i].val)
			stralloc_catf(sa, "%s%s", list[i].key, delim);

	if (sa->len > 0)
		sa->len -= str_len(delim);

	buf = stralloc_finalize(sa);

	stralloc_free(sa);
	return buf;
}

const char *flist64_getkey(const flist64_t list[], uint64_t val)
{
	int i;

	for (i = 0; list[i].key; i++)
		if (list[i].val == val)
			return list[i].key;

	return 0;
}

uint64_t flist64_getval(const flist64_t list[], const char *key)
{
	int i;

	for (i = 0; list[i].key; i++)
		if (str_equal(list[i].key, key))
			return list[i].val;

	return 0;
}

int flist64_from_str(const char *str, const flist64_t list[],
		uint64_t *flags, uint64_t *mask,
		char clmod, const char *delim)
{
	char *token;
	int clear = 0;
	uint64_t cur_flag;

	strtok_t _st, *st = &_st, *p;

	if (!strtok_init_str(st, str, delim, 0))
		return -1;

	strtok_for_each(st, p) {
		token = p->token;

		if (*token == clmod)
			clear = 1;

		cur_flag = flist64_getval(list, token+clear);

		if (!cur_flag) {
			strtok_free(st);
			return -1;
		}

		if (clear) {
			*flags &= ~cur_flag;
			*mask  |=  cur_flag;
		} else {
			*flags |=  cur_flag;
			*mask  |=  cur_flag;
		}
	}

	strtok_free(st);

	return 0;
}

char *flist64_to_str(const flist64_t list[], uint64_t val, const char *delim)
{
	int i;
	char *buf;
	stralloc_t _sa, *sa = &_sa;

	stralloc_init(sa);

	for (i = 0; list[i].key; i++)
		if (val & list[i].val)
			stralloc_catf(sa, "%s%s", list[i].key, delim);

	if (sa->len > 0)
		sa->len -= str_len(delim);

	buf = stralloc_finalize(sa);

	stralloc_free(sa);
	return buf;
}
