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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "log.h"
#include "mem.h"
#include "str.h"

static
int str_check_t(void)
{
	int i, res, rc = 0;
	char allchars[128];

	struct test {
		char *str;
		int allowed;
		int res;
	} T[] = {
		{ NULL,              0,        1 },
		{ NULL,             ~0,        1 },
		{ "",                0,        1 },
		{ "",               ~0,        1 },
		{ "0123456789abcd", CC_ALNUM,  1 },
		{ "0123456789abcd", CC_ALPHA,  0 },
		{ "abcdefghijklmn", CC_ALPHA,  1 },
		{ allchars,         CC_ASCII,  1 },
		{ " \t \t  \t \t ", CC_BLANK,  1 },
		{ " \t \t\v\t \t ", CC_BLANK,  0 },
		{ "01234567891234", CC_DIGIT,  1 },
		{ "0123456789abcd", CC_DIGIT,  0 },
		{ "abcdefghijklmn", CC_LOWER,  1 },
		{ "!\"#$%&'()*+,,", CC_PUNCT,  1 },
		{ "/:;<=>?@[\\]^_", CC_PUNCT,  1 },
		{ "`{|}~",          CC_PUNCT,  1 },
		{ "/:;<=>a@[\\]^_", CC_PUNCT,  0 },
		{ " \t \t  \t \t ", CC_SPACE,  1 },
		{ "\t\t\t\t\t\t\t", CC_SPACE,  1 },
		{ "\t\t\taa\t\t\t", CC_SPACE,  0 },
		{ "abcdefghijklmn", CC_UPPER,  0 },
		{ "aBcdefgHijklmn", CC_UPPER,  0 },
		{ "ABCDEFGHIJKLMN", CC_UPPER,  1 },
		{ "024A6789ABCDEF", CC_XDIGIT, 1 },
		{ "024A6789ABCDEG", CC_XDIGIT, 0 },
		{ "0x4A6789ABCDEF", CC_XDIGIT, 0 },
		{ allchars,          0,        0 },
		{ allchars,         ~0,        1 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < 127; i++)
		allchars[i] = i + 1;

	allchars[127] = 0;

	for (i = 0; i < TS; i++) {
		res = str_check(T[i].str, T[i].allowed);
		
		if (res != T[i].res)
			rc += log_error("[%s/%02d] E[%d] R[%d]",
			                __FUNCTION__, i,
			                T[i].res, res);
	}

	return rc;
}

static
int str_path_basedirname_t(void)
{
	int i, rc = 0;
	char *dirname, *basename;

	struct test {
		char *path;
		char *dirname;
		char *basename;
	} T[] = {
		{ "/usr/lib", "/usr",  "lib" },
		{ "/usr/",    "/",     "usr" },
		{ "usr",      ".",     "usr" },
		{ "/",        "/",     "/"   },
		{ ".",        ".",     "."   },
		{ "..",       ".",     ".."  },
		{ "/////",    "/",     "/"   },
		{ "//f//",    "/",     "f"   },
		{ "//f//g",   "/f",    "g"   },
		{ "../..//.", "../..", "."   },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		basename = str_path_basename(T[i].path);
		dirname  = str_path_dirname(T[i].path);
		
		if (basename && dirname &&
				(strcmp(basename, T[i].basename) != 0 ||
				strcmp(dirname, T[i].dirname) != 0))
			rc += log_error("[%s/%02d] E[%s,%s] R[%s,%s]",
					__FUNCTION__, i,
					T[i].dirname, T[i].basename,
					dirname, basename);
		
		if (basename)
			mem_free(basename);

		if (dirname)
			mem_free(dirname);
	}

	return rc;
}

static
int str_path_concat_t(void)
{
	int i, rc = 0;
	char *path;

	struct test {
		char *dirname;
		char *basename;
		char *path;
	} T[] = {
		{ "",        "",     NULL },
		{ "/",       "",     "//" }, /* ugly but valid */
		{ "",        "/",    NULL },
		{ "/",       "/",    "///" }, /* ugly but valid */
		{ ".",       "/",    ".//" }, /* ugly but valid */
		{ "..",      "/",    "..//" }, /* ugly but valid */
		{ "/",       ".",    NULL },
		{ "/",       "..",   NULL },
		{ "/a/b/c",  "d",    "/a/b/c/d" },
		{ "/a/../c", "d",    "/a/../c/d" },
		{ "/a/../c", "../d", NULL },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		path = str_path_concat(T[i].dirname, T[i].basename);
		
		if (path == NULL && T[i].path == NULL)
			continue;
		
		if ((path == NULL && T[i].path != NULL) ||
		    (path != NULL && T[i].path == NULL) ||
		    strcmp(path, T[i].path) != 0)
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].path, path);
		
		if (path)
			mem_free(path);
	}

	return rc;
}

static
int str_path_isabs_t(void)
{
	int i, res, rc = 0;
	char *buf;

	struct test {
		char *str;
		int res;
	} T[] = {
		{ "",           0 },
		{ "   ",        0 },
		{ "abc",        0 },
		{ "/",          1 },
		{ "/////",      1 },
		{ "/a/b/c/d/e", 1 },
		{ "/a/b///d/e", 1 },
		{ "/a/../d/e",  0 },
		{ "/a/./d/e",   0 },
		{ "/a/\n/d/e",  0 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		buf = strdup(T[i].str);
		res = str_path_isabs(buf);
		mem_free(buf);
		
		if (res != T[i].res)
			rc += log_error("[%s/%02d] E[%d] R[%d]",
			                __FUNCTION__, i,
			                T[i].res, res);
	}

	return rc;
}

static
int str_path_isdot_t(void)
{
	int i, res, rc = 0;
	char *buf;

	struct test {
		char *str;
		int res;
	} T[] = {
		{ "",           0 },
		{ "   ",        0 },
		{ "abc",        0 },
		{ "/",          0 },
		{ ".",          1 },
		{ "./../a/b",   1 },
		{ "//.///",     1 },
		{ "/a/b/c/d/e", 0 },
		{ "/a/b///d/e", 0 },
		{ "/a/../d/e",  1 },
		{ "/a/./d/e",   1 },
		{ "/a/\n/d/e",  0 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		buf = strdup(T[i].str);
		res = str_path_isdot(buf);
		mem_free(buf);
		
		if (res != T[i].res)
			rc += log_error("[%s/%02d] E[%d] R[%d]",
			                __FUNCTION__, i,
			                T[i].res, res);
	}

	return rc;
}

static
int str_tolower_t(void)
{
	int rc = 0;

	char orig[]  = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	               "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	char lower[] = "!\"#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz"
	               "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	char *buf = str_tolower(orig);

	if (strcmp(buf, lower))
		rc += log_error("[%s]\nE[%s]\nR[%s]", __FUNCTION__, lower, buf);

	return rc;
}

static
int str_toupper_t(void)
{
	int rc = 0;

	char orig[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	               "[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	char upper[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	               "[\\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~";

	char *buf = str_toupper(orig);

	if (strcmp(buf, upper))
		rc += log_error("[%s]\nE[%s]\nR[%s]", __FUNCTION__, upper, buf);

	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;

	log_options_t log_options = {
		.log_ident  = "str",
		.log_dest  = LOGD_STDERR,
		.log_opts  = LOGO_PRIO|LOGO_IDENT,
	};

	log_init(&log_options);

	rc += str_check_t();
	rc += str_path_basedirname_t();
	rc += str_path_concat_t();
	rc += str_path_isabs_t();
	rc += str_path_isdot_t();
	rc += str_tolower_t();
	rc += str_toupper_t();

	log_close();

	return rc;
}
