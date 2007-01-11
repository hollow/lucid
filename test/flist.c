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
#include "mem.h"

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
int flist32_from_str_t(void)
{
	int i, ret, rc = 0;
	uint32_t flags, mask;
	
	struct test {
		char *str;
		int ret;
		uint32_t flags;
		uint32_t mask;
	} T[] = {
		{ NULL,        0, 0,             0 },
		{ "",          0, 0,             0 },
		{ "A",         0, NODE_A,        NODE_A },
		{ "A,B",       0, NODE_A|NODE_B, NODE_A|NODE_B },
		{ "A,B,~C",    0, NODE_A|NODE_B, NODE_A|NODE_B|NODE_C },
		{ "A,~B,D,C", -1, NODE_A,        NODE_A|NODE_B },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		flags = mask = 0;
		
		ret = flist32_from_str(T[i].str, list32, &flags, &mask, '~', ",");
		
		if (ret   != T[i].ret ||
		    flags != T[i].flags  ||
		    mask  != T[i].mask)
			rc += log_error("[%s/%02d] E[%d,%#.8x,%#.8x] R[%d,%#.8x,%#.8x]",
			                __FUNCTION__, i,
			                T[i].ret, T[i].flags, T[i].mask,
			                ret,      flags,      mask);
	}
	
	return rc;
}

static
int flist64_from_str_t(void)
{
	int i, ret, rc = 0;
	uint64_t flags, mask;
	
	struct test {
		char *str;
		int ret;
		uint64_t flags;
		uint64_t mask;
	} T[] = {
		{ NULL,        0, 0,             0 },
		{ "",          0, 0,             0 },
		{ "A",         0, NODE_A,        NODE_A },
		{ "A,D",       0, NODE_A|NODE_D, NODE_A|NODE_D },
		{ "A,D,~E",    0, NODE_A|NODE_D, NODE_A|NODE_D|NODE_E },
		{ "A,~D,F,E", -1, NODE_A,        NODE_A|NODE_D },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		flags = mask = 0;
		
		ret = flist64_from_str(T[i].str, list64, &flags, &mask, '~', ",");
		
		if (ret   != T[i].ret ||
		    flags != T[i].flags  ||
		    mask  != T[i].mask)
			rc += log_error("[%s/%02d] E[%d,%#.16x,%#.16x] R[%d,%#.16x,%#.16x]",
			                __FUNCTION__, i,
			                T[i].ret, T[i].flags, T[i].mask,
			                ret,      flags,      mask);
	}
	
	return rc;
}

static
int flist32_to_str_t(void)
{
	int i, rc = 0;
	char *str = NULL;
	
	struct test {
		uint32_t flags;
		char *str;
	} T[] = {
		{ 0, "" },
		{ NODE_B,        "B" },
		{ NODE_A|0x2,    "A" },
		{ NODE_A|NODE_B, "A,B" },
		{ ~0UL,          "A,B,C" },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		if (str)
			mem_free(str);
		
		str = flist32_to_str(list32, T[i].flags, ",");
		
		if (strcmp(str, T[i].str))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].str, str);
	}
	
	return rc;
}

static
int flist64_to_str_t(void)
{
	int i, rc = 0;
	char *str = NULL;
	
	struct test {
		uint64_t flags;
		char *str;
	} T[] = {
		{ 0, "" },
		{ NODE_B,        "B" },
		{ NODE_A|0x2,    "A" },
		{ NODE_A|NODE_B, "A,B" },
		{ ~0ULL,         "A,B,C,D,E" },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		if (str)
			mem_free(str);
		
		str = flist64_to_str(list64, T[i].flags, ",");
		
		if (strcmp(str, T[i].str))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].str, str);
	}
	
	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;
	
	log_options_t log_options = {
		.ident  = "flist",
		.stderr = true,
		.time   = false,
	};
	
	log_init(&log_options);
	
	rc += flist32_from_str_t();
	rc += flist64_from_str_t();
	rc += flist32_to_str_t();
	rc += flist64_to_str_t();
	
	log_close();
	
	return rc;
}
