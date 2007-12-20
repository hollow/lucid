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

#include "flist.h"
#include "log.h"

#define NODE_A 0x0001
#define NODE_B 0x0008
#define NODE_C 0x0800
#define NODE_D (1ULL << 48)
#define NODE_E (1ULL << 56)

FLIST32_START(list32)
FLIST32_NODE(NODE, A)
FLIST32_NODE(NODE, B)
FLIST32_NODE(NODE, C)
FLIST32_END

FLIST64_START(list64)
FLIST64_NODE(NODE, A)
FLIST64_NODE(NODE, B)
FLIST64_NODE(NODE, C)
FLIST64_NODE(NODE, D)
FLIST64_NODE(NODE, E)
FLIST64_END

static
int flist32_decode_t(void)
{
	int i, ret, rc = 0;
	flag32_t flag32;

	struct test {
		const char *str;
		int ret;
		flag32_t flag32;
	} T[] = {
		{ NULL,        0, { 0,             0 } },
		{ "",          0, { 0,             0 } },
		{ "A",         0, { NODE_A,        NODE_A } },
		{ "A,B",       0, { NODE_A|NODE_B, NODE_A|NODE_B } },
		{ "A,B,~C",    0, { NODE_A|NODE_B, NODE_A|NODE_B|NODE_C } },
		{ "A,~B,D,C", -1, { NODE_A,        NODE_A|NODE_B } },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		flag32.flag = flag32.mask = 0;

		ret = flist32_decode(T[i].str, list32, &flag32, '~', ",");

		if (ret   != T[i].ret ||
		    flag32.flag != T[i].flag32.flag ||
		    flag32.mask != T[i].flag32.mask)
			rc += log_error("[%s/%02d] E[%d,%#.8x,%#.8x] R[%d,%#.8x,%#.8x]",
			                __FUNCTION__, i,
			                T[i].ret, T[i].flag32.flag, T[i].flag32.mask,
			                ret,      flag32.flag,      flag32.mask);
	}

	return rc;
}

static
int flist64_decode_t(void)
{
	int i, ret, rc = 0;
	flag64_t flag64;

	struct test {
		const char *str;
		int ret;
		flag64_t flag64;
	} T[] = {
		{ NULL,        0, { 0,             0 } },
		{ "",          0, { 0,             0 } },
		{ "A",         0, { NODE_A,        NODE_A } },
		{ "A,D",       0, { NODE_A|NODE_D, NODE_A|NODE_D } },
		{ "A,D,~E",    0, { NODE_A|NODE_D, NODE_A|NODE_D|NODE_E } },
		{ "A,~D,F,E", -1, { NODE_A,        NODE_A|NODE_D } },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		flag64.flag = flag64.mask = 0;

		ret = flist64_decode(T[i].str, list64, &flag64, '~', ",");

		if (ret   != T[i].ret ||
		    flag64.flag != T[i].flag64.flag ||
		    flag64.mask != T[i].flag64.mask)
			rc += log_error("[%s/%02d] E[%d,%#.16x,%#.16x] R[%d,%#.16x,%#.16x]",
			                __FUNCTION__, i,
			                T[i].ret, T[i].flag64.flag, T[i].flag64.mask,
			                ret,      flag64.flag,      flag64.mask);
	}

	return rc;
}

static
int flist32_encode_t(void)
{
	int i, rc = 0;
	char *str = NULL;

	struct test {
		flag32_t flag32;
		const char *str;
	} T[] = {
		{ { 0,              0 },              "" },
		{ { NODE_B,         NODE_B },         "B" },
		{ { NODE_A|0x2,     NODE_A|0x2 },     "A" },
		{ { NODE_A|NODE_B,  NODE_A|NODE_B },  "A,B" },
		{ { NODE_B,         NODE_A|NODE_B },  "~A,B" },
		{ { (uint32_t)~0UL, (uint32_t)~0UL }, "A,B,C" },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		str = flist32_encode(list32, &(T[i].flag32), '~', ",");

		if (strcmp(str, T[i].str))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].str, str);

		if (str)
			free(str);
	}

	return rc;
}

static
int flist64_encode_t(void)
{
	int i, rc = 0;
	char *str = NULL;

	struct test {
		flag64_t flag64;
		const char *str;
	} T[] = {
		{ { 0,               0 },               "" },
		{ { NODE_B,          NODE_B },          "B" },
		{ { NODE_A|0x2,      NODE_A|0x2 },      "A" },
		{ { NODE_A|NODE_B,   NODE_A|NODE_B },   "A,B" },
		{ { NODE_B,          NODE_A|NODE_B },   "~A,B" },
		{ { (uint64_t)~0ULL, (uint64_t)~0ULL }, "A,B,C,D,E" },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		str = flist64_encode(list64, &(T[i].flag64), '~', ",");

		if (strcmp(str, T[i].str))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].str, str);

		if (str)
			free(str);
	}

	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;

	log_options_t log_options = {
		.log_ident  = "flist",
		.log_dest  = LOGD_STDERR,
		.log_opts  = LOGO_PRIO|LOGO_IDENT,
	};

	log_init(&log_options);

	rc += flist32_decode_t();
	rc += flist64_decode_t();
	rc += flist32_encode_t();
	rc += flist64_encode_t();

	log_close();

	return rc;
}
