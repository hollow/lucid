// Copyright (C) 2005      Felix von Leitner <felix-libowfat@fefe.de>
//               2006-2007 Benedikt Böhm <hollow@gentoo.org>
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
#include <errno.h>

#include "mem.h"
#include "printf.h"
#include "str.h"
#include "stralloc.h"

void stralloc_init(stralloc_t *sa)
{
	sa->s   = 0;
	sa->len = sa->a = 0;
}

void stralloc_zero(stralloc_t *sa)
{
	sa->len = 0;
}

int stralloc_ready(stralloc_t *sa, size_t len)
{
	size_t wanted = len + (len >> 3) + 30;
	char *tmp;

	if (!sa->s || sa->a < len) {
		if (!(tmp = mem_realloc(sa->s, wanted)))
			return -1;

		sa->a = wanted;
		sa->s = tmp;
	}

	return 0;
}

int stralloc_readyplus(stralloc_t *sa, size_t len)
{
	if (sa->s) {
		if (sa->len + len < len)
			return errno = EINVAL, -1;

		return stralloc_ready(sa, sa->len + len);
	}

	return stralloc_ready(sa, len);
}

char *stralloc_finalize(stralloc_t *sa)
{
	char *buf = mem_alloc(sa->len + 1);

	if (!buf)
		return 0;

	mem_cpy(buf, sa->s, sa->len);
	return buf;
}

void stralloc_free(stralloc_t *sa)
{
	if (sa->s)
		mem_free(sa->s);

	sa->s = 0;
}

int stralloc_copyb(stralloc_t *dst, const char *src, size_t len)
{
	if (stralloc_ready(dst, len) == -1)
		return -1;

	mem_cpy(dst->s, src, len);
	dst->len = len;
	return 0;
}

int stralloc_copys(stralloc_t *dst, const char *src)
{
	return stralloc_copyb(dst, src, str_len(src));
}

int stralloc_copy(stralloc_t *dst, const stralloc_t *src)
{
	return stralloc_copyb(dst, src->s, src->len);
}

int stralloc_catb(stralloc_t *dst, const char *src, size_t len)
{
	if (stralloc_readyplus(dst, len) == -1)
		return -1;

	mem_cpy(dst->s + dst->len, src, len);
	dst->len += len;
	return 0;
}

int stralloc_catf(stralloc_t *dst, const char *fmt, ...)
{
	va_list ap;
	char *buf;
	int rc;

	va_start(ap, fmt);

	if (_lucid_vasprintf(&buf, fmt, ap) == -1) {
		va_end(ap);
		return -1;
	}

	va_end(ap);

	rc = stralloc_cats(dst, buf);

	mem_free(buf);

	return rc;
}

int stralloc_catm(stralloc_t *dst, ...)
{
	va_list ap;
	char *s;

	va_start(ap, dst);

	while ((s = va_arg(ap, char *))) {
		if (stralloc_cats(dst, s) == -1) {
			va_end(ap);
			return -1;
		}
	}

	va_end(ap);
	return 0;
}

int stralloc_cats(stralloc_t *dst, const char *src)
{
	return stralloc_catb(dst, src, str_len(src));
}

int stralloc_cat(stralloc_t *dst, const stralloc_t *src)
{
	return stralloc_catb(dst, src->s, src->len);
}

int stralloc_cmp(const stralloc_t *a, const stralloc_t *b)
{
	size_t i, j;

	for (i = 0;; ++i) {
		if (i == a->len)
			return i == b->len ? 0 : -1;

		if (i == b->len)
			return 1;

		if ((j = ((unsigned char)(a->s[i]) - (unsigned char)(b->s[i]))))
			return j;
	}

	return j;
}
