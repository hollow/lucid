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

#include "char.h"
#include "cext.h"
#include "printf.h"
#include "str.h"
#include "strtok.h"

int str_check(const char *str, int allowed)
{
	int i, n;

	if (!str)
		return 1;

	n = str_len(str);

	for (i = 0; i < n; i++) {
		if (allowed & CC_ALNUM  && char_isalnum (str[i])) continue;
		if (allowed & CC_ALPHA  && char_isalpha (str[i])) continue;
		if (allowed & CC_ASCII  && char_isascii (str[i])) continue;
		if (allowed & CC_BLANK  && char_isblank (str[i])) continue;
		if (allowed & CC_CNTRL  && char_iscntrl (str[i])) continue;
		if (allowed & CC_DIGIT  && char_isdigit (str[i])) continue;
		if (allowed & CC_GRAPH  && char_isgraph (str[i])) continue;
		if (allowed & CC_LOWER  && char_islower (str[i])) continue;
		if (allowed & CC_PRINT  && char_isprint (str[i])) continue;
		if (allowed & CC_PUNCT  && char_ispunct (str[i])) continue;
		if (allowed & CC_SPACE  && char_isspace (str[i])) continue;
		if (allowed & CC_UPPER  && char_isupper (str[i])) continue;
		if (allowed & CC_XDIGIT && char_isxdigit(str[i])) continue;

		return 0;
	}

	return 1;
}

char *str_chr(const char *str, int c, int n)
{
	for (; n; str++, n--)
		if (*str == c)
			return (char *) str;

	return 0;
}

int str_cmp(const char *str1, const char *str2)
{
	if (!str1 && !str2)
		return 0;

	if (!str1 || !str2)
		return 1;

	while (*str1 && *str2 && *str1 == *str2)
		str1++, str2++;

	return *str1 - *str2;
}

int str_cmpn(const char *str1, const char *str2, int n)
{
	if (!str1 && !str2)
		return 0;

	if (!str1 || !str2)
		return 1;

	while (--n && *str1 && *str2 && *str1 == *str2)
		str1++, str2++;

	return *str1 - *str2;
}

char *str_cpy(char *dst, const char *src)
{
	return memcpy(dst, src, str_len(src) + 1);
}

char *str_cpyn(char *dst, const char *src, int n)
{
	int len = str_len(src) + 1;
	return memcpy(dst, src, n > len ? len : n);
}

char *str_dup(const char *str)
{
	return memdup(str, str_len(str) + 1);
}

int str_equal(const char *str1, const char *str2)
{
	return str_cmp(str1, str2) == 0;
}

int str_len(const char *str)
{
	int i = 0;

	while (*str++)
		i++;

	return i;
}

char *str_rchr(const char *str, int c, int n)
{
	for (str += n - 1; n; str--, n--)
		if (*str == c)
			return (char *) str;

	return 0;
}

char *str_str(const char *str, const char *needle)
{
	int i;
	int slen = str_len(str);
	int nlen = str_len(needle);

	if (nlen < 1)
		return (char *) str;

	if (nlen > slen)
		return 0;

	for (i = slen - nlen + 1; i; i--) {
		if (memcmp(str, needle, nlen) == 0)
			return (char *) str;

		str++;
	}

	return 0;
}

char *str_flatten(char *str)
{
	char *p = str;

	while (*p) {
		if (*p == '\n' || *p == '\r')
			*p = ' ';
		p++;
	}

	return str;
}

char *str_tolower(char *str)
{
	char *p = str;

	while (*p) {
		char_tolower(*p);
		p++;
	}

	return str;
}

char *str_toupper(char *str)
{
	char *p = str;

	while (*p) {
		char_toupper(*p);
		p++;
	}

	return str;
}

char *str_path_basename(const char *path)
{
	/* empty string */
	if (str_isempty(path))
		return str_dup(".");

	/* skip prefixing '/' */
	while (*path && *path == '/')
		path++;

	/* string consisting entirely of '/' */
	if (!*path)
		return str_dup("/");

	char *p, *buf = str_dup(path);

	while ((p = str_rchr(buf, '/', str_len(buf)))) {
		/* remove trailing lash */
		if (p[1] == 0 && p != buf)
			*p = 0;

		/* no trailing slash anymore */
		else
			break;
	}

	char *bn;

	/* if a non-trailing slash was found, return everything after it */
	if (p)
		bn = str_dup(p + 1);

	/* otherwise buf already contains basename */
	else
		bn = str_dup(buf);

	free(buf);

	return bn;
}

char *str_path_dirname(const char *path)
{
	/* empty string or '..' */
	if (str_isempty(path) || str_equal(path, ".."))
		return str_dup(".");

	/* skip prefixing '/' but preserve exactly one */
	while (*path && *(path+1) && *path == '/' && *(path+1) == '/')
		path++;

	int found = 0;
	char *p, *buf = str_dup(path);

	while ((p = str_rchr(buf, '/', str_len(buf)))) {
		/* remove trailing slash */
		if (p[1] == 0 && p != buf)
			*p = 0;

		/* no basename was found until yet */
		else if (!found) {
			*p = 0;
			found = 1;
		}

		/* a basename was found and no trailing slash anymore */
		else
			break;
	}

	char *dn;

	/* path consists only of basename and slashes */
	if (str_isempty(buf))
		dn = str_dup("/");

	/* path is relative or absolute, basename was stripped */
	else if (p)
		dn = str_dup(buf);

	/* path is relative, no basename was stripped */
	else
		dn = str_dup(".");

	free(buf);

	return dn;
}

char *str_path_concat(const char *dirname, const char *basename)
{
	char *path = 0;

	if (str_len(basename) > 1 && memcmp(basename, "./", 2) == 0)
		basename += 2;

	if (str_isempty(dirname) || str_path_isdot(basename))
		return 0;

	_lucid_asprintf(&path, "%s/%s", dirname, basename);

	return path;
}

int str_path_isabs(const char *str)
{
	int abs = 1;

	if (str_isempty(str))
		return 0;

	if (*str != '/')
		return 0;

	strtok_t _st, *st = &_st, *p;

	if (!strtok_init_str(st, str, "/", 0))
		return -1;

	strtok_for_each(st, p) {
		if (str_equal(p->token, ".") || str_equal(p->token, "..") ||
		    !str_isgraph(p->token)) {
			abs = 0;
			break;
		}
	}

	strtok_free(st);

	return abs;
}

int str_path_isdot(const char *str)
{
	int found = 0;

	if (str_isempty(str))
		return 0;

	strtok_t _st, *st = &_st, *p;

	if (!strtok_init_str(st, str, "/", 0))
		return 0;

	strtok_for_each(st, p) {
		if (str_equal(p->token, ".") || str_equal(p->token, "..")) {
			found = 1;
			break;
		}
	}

	strtok_free(st);

	return found;
}

static inline
int char_todigit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';

	else if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;

	else if (c >= 'a' && c <= 'z')
		return c - 'a' + 10;

	else
		return -1;
}

int str_toumax(const char *str, unsigned long long int *val, int base, int n)
{
	unsigned long long int v = 0;
	const char *p = str;
	int d, minus = 0;
	char c;

	while (n && char_isspace((unsigned char) *p)) {
		p++;
		n--;
	}

	/* Single optional + or - */
	if (n) {
		c = *p;

		if (c == '-' || c == '+') {
			minus = (c == '-');
			p++;
			n--;
		}
	}

	if (base == 0) {
		if (n >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
			n -= 2;
			p += 2;
			base = 16;
		}

		else if (n >= 1 && p[0] == '0') {
			n--;
			p++;
			base = 8;
		}

		else {
			base = 10;
		}
	}

	else if (base == 16) {
		if (n >= 2 && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
			n -= 2;
			p += 2;
		}
	}

	while (n && (d = char_todigit(*p)) >= 0 && d < base) {
		v = v * base + d;
		n--;
		p++;
	}

	if (p - str > 0)
		*val = minus ? -v : v;

	return p - str;
}

