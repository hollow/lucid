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

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "flist.h"
#include "log.h"
#include "mem.h"
#include "rtti.h"
#include "str.h"

static
int rtti_bool32_encode_t(void)
{
	const rtti_t bool32_type = RTTI_BOOL_TYPE(int32_t);
	int i, ret, rc = 0;
	char *buf;

	struct test {
		int32_t n;
		const char *s;
		int ret;
	} T[] = {
		{ 0,    "false", 5 },
		{ 12,   "true",  4 },
		{ 127,  "true",  4 },
		{ -145, "true",  4 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		ret = rtti_encode(&bool32_type, NULL, &T[i].n, &buf);

		if (ret  != T[i].ret || !str_equal(buf, T[i].s))
			rc += log_error("[%s/%02d] E[%d,%s] R[%d,%s]",
					__FUNCTION__, i, T[i].ret, T[i].s, ret, buf);

		if (buf)
			mem_free(buf);
	}

	return rc;
}

static
int rtti_bool32_decode_t(void)
{
	const rtti_t bool32_type = RTTI_BOOL_TYPE(int32_t);
	int i, ret, rc = 0;
	int32_t buf;

	struct test {
		int32_t n;
		const char *s;
		int ret;
		int e;
	} T[] = {
		{ 0,    "",      0, EINVAL },
		{ 0,    "abc",   0, EINVAL },
		{ 1,    "true",  4, 0 },
		{ 0,    "false", 5, 0 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		int k = i;
		errno = 0;
		buf = 0;
		ret = rtti_decode(&bool32_type, NULL, T[k].s, &buf);

		if (ret != T[k].ret || buf != T[k].n || errno != T[k].e)
			rc += log_error("[%s/%02d] E[%d,%"PRIi32",%d] R[%d,%"PRIi32",%d]",
					__FUNCTION__, k, T[k].ret, T[k].n, T[k].e, ret, buf, errno);

		i = k;
	}

	return rc;
}

#define NODE_A 0x0001
#define NODE_B 0x0008
#define NODE_C 0x0800

FLIST32_START(test_list)
FLIST32_NODE(NODE, A)
FLIST32_NODE(NODE, B)
FLIST32_NODE(NODE, C)
FLIST32_END

static
int rtti_flist_encode_t(void)
{
	const rtti_t flist_type = RTTI_FLIST_TYPE(32, test_list, ",", '~');
	int i, ret, rc = 0;
	char *buf;

	struct test {
		flag32_t flag32;
		const char *s;
		int ret;
	} T[] = {
		{ { 0, 0 },                         "\"\"",     2 },
		{ { NODE_A, NODE_A },               "\"A\"",    3 },
		{ { NODE_A|NODE_B, NODE_A|NODE_B }, "\"A,B\"",  5 },
		{ { NODE_B, NODE_A|NODE_B },        "\"~A,B\"", 6 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		ret = rtti_encode(&flist_type, NULL, &T[i].flag32, &buf);

		if (ret  != T[i].ret || !str_equal(buf, T[i].s))
			rc += log_error("[%s/%02d] E[%d,%s] R[%d,%s]",
					__FUNCTION__, i, T[i].ret, T[i].s, ret, buf);

		if (buf)
			mem_free(buf);
	}

	return rc;
}

static
int rtti_flist_decode_t(void)
{
	const rtti_t flist_type = RTTI_FLIST_TYPE(32, test_list, ",", '~');
	int i, ret, rc = 0;
	flag32_t buf;

	struct test {
		flag32_t flag32;
		const char *s;
		int ret;
	} T[] = {
		{ { 0, 0 },                         "\"\"",     2 },
		{ { NODE_A, NODE_A },               "\"A\"",    3 },
		{ { NODE_A|NODE_B, NODE_A|NODE_B }, "\"A,B\"",  5 },
		{ { NODE_B, NODE_A|NODE_B },        "\"~A,B\"", 6 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		buf.flag = buf.mask = 0;
		ret = rtti_decode(&flist_type, NULL, T[i].s, &buf);

		if (ret != T[i].ret || buf.flag != T[i].flag32.flag || buf.mask != T[i].flag32.mask)
			rc += log_error("[%s/%02d] E[%d,%"PRIx32",%"PRIx32"] R[%d,%"PRIx32",%"PRIx32"]",
					__FUNCTION__, i, T[i].ret, T[i].flag32.flag, T[i].flag32.mask, ret, buf.flag, buf.mask);
	}

	return rc;
}

static
int rtti_int8_encode_t(void)
{
	const rtti_t int8_type = RTTI_INT_TYPE(int8_t, PRIi8, 1);
	int i, ret, rc = 0;
	char *buf;

	struct test {
		int8_t n;
		const char *s;
		int ret;
	} T[] = {
		{ 0,    "0",    1 },
		{ 12,   "12",   2 },
		{ 127,  "127",  3 },
		{ -128, "-128", 4 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		ret = rtti_encode(&int8_type, NULL, &T[i].n, &buf);

		if (ret  != T[i].ret || !str_equal(buf, T[i].s))
			rc += log_error("[%s/%02d] E[%d,%s] R[%d,%s]",
					__FUNCTION__, i, T[i].ret, T[i].s, ret, buf);

		if (buf)
			mem_free(buf);
	}

	return rc;
}

static
int rtti_int8_decode_t(void)
{
	const rtti_t int8_type = RTTI_INT_TYPE(int8_t, PRIi8, 1);
	int i, ret, rc = 0;
	int8_t buf;

	struct test {
		int8_t n;
		const char *s;
		int ret;
		int e;
	} T[] = {
		{ 0,    "",     0, EINVAL },
		{ 0,    "abc",  0, EINVAL },
		{ 0,    "0.0",  1, 0 },
		{ 0,    "0",    1, 0 },
		{ 12,   "12",   2, 0 },
		{ 127,  "127",  3, 0 },
		{ -128, "-128", 4, 0 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		errno = 0;
		ret = rtti_decode(&int8_type, NULL, T[i].s, &buf);

		if (ret != T[i].ret || buf != T[i].n || errno != T[i].e)
			rc += log_error("[%s/%02d] E[%d,%"PRIi8",%d] R[%d,%"PRIi8",%d]",
					__FUNCTION__, i, T[i].ret, T[i].n, T[i].e, ret, buf, errno);
	}

	return rc;
}

struct test_struct {
	int8_t *i;
	const char *s;
	int8_t a[2];
};

static
int rtti_struct_encode_t(void)
{
	const rtti_t int8_type = RTTI_INT_TYPE(int8_t, PRIi8, 1);
	const rtti_t int8p_type = RTTI_POINTER_TYPE(&int8_type);
	const rtti_t int8_array_type = RTTI_ARRAY_TYPE(&int8_type, 2);
	const rtti_t string_type = RTTI_STRING_TYPE(1, 1);
	const rtti_field_t fields[] = {
		RTTI_STRUCT_FIELD(test_struct, i, &int8p_type),
		RTTI_STRUCT_FIELD(test_struct, s, &string_type),
		RTTI_STRUCT_FIELD(test_struct, a, &int8_array_type),
		RTTI_STRUCT_FIELD_END
	};
	const rtti_t struct_type = RTTI_STRUCT_TYPE(test_struct, fields);
	int i, ret, rc = 0;
	char *buf;
	int8_t ival = 23;

	struct test {
		struct test_struct st;
		const char *s;
		int ret;
	} T[] = {
		{ { &ival, "foo", { 4, 5 } } , "{\"i\":23,\"s\":\"foo\",\"a\":[4,5]}", 28 },
		{ { &ival, NULL,  { 4, 5 } } , "{\"i\":23,\"s\":null,\"a\":[4,5]}",    27 },
		{ { &ival, "foo", { 4 }    } , "{\"i\":23,\"s\":\"foo\",\"a\":[4,0]}", 28 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		ret = rtti_encode(&struct_type, NULL, &T[i].st, &buf);

		if (ret  != T[i].ret || !str_equal(buf, T[i].s))
			rc += log_error("[%s/%02d] E[%d,%s] R[%d,%s]",
					__FUNCTION__, i, T[i].ret, T[i].s, ret, buf);

		if (buf)
			mem_free(buf);
	}

	return rc;
}

static
int rtti_struct_decode_t(void)
{
	const rtti_t int8_type = RTTI_INT_TYPE(int8_t, PRIi8, 1);
	const rtti_t int8p_type = RTTI_POINTER_TYPE(&int8_type);
	const rtti_t int8_array_type = RTTI_ARRAY_TYPE(&int8_type, 2);
	const rtti_t string_type = RTTI_STRING_TYPE(1, 1);
	const rtti_field_t fields[] = {
		RTTI_STRUCT_FIELD(test_struct, i, &int8p_type),
		RTTI_STRUCT_FIELD(test_struct, s, &string_type),
		RTTI_STRUCT_FIELD(test_struct, a, &int8_array_type),
		RTTI_STRUCT_FIELD_END
	};
	const rtti_t struct_type = RTTI_STRUCT_TYPE(test_struct, fields);
	int i, ret, rc = 0;
	int8_t ival2 = 0, ival = 25;
	struct test_struct buf = { &ival2, NULL, { 0, 0 } };

	struct test {
		struct test_struct st;
		const char *s;
		int ret;
	} T[] = {
		{ { &ival, "foo", { 4, 5 } } , "{\"i\":25,\"s\":\"foo\",\"a\":[4,5]}", 28 },
		{ { &ival, NULL,  { 4, 5 } } , "{\"i\":25,\"s\":null,\"a\":[4,5]}",    27 },
		{ { &ival, "foo", { 4 }    } , "{\"i\":25,\"s\":\"foo\",\"a\":[4]}",   26 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		errno = 0;
		ret = rtti_decode(&struct_type, NULL, T[i].s, &buf);

		if (ret != T[i].ret || *(T[i].st.i) != *(buf.i) || !str_equal(T[i].st.s, buf.s) ||
				T[i].st.a[0] != buf.a[0] || T[i].st.a[1] != buf.a[1])
			rc += log_error("[%s/%02d] E[%d,{%"PRIi8",%s,[%"PRIi8",%"PRIi8"]}] R[%d,{%"PRIi8",%s,[%"PRIi8",%"PRIi8"]}]",
					__FUNCTION__, i,
					T[i].ret, *(T[i].st.i), T[i].st.s, T[i].st.a[0], T[i].st.a[1],
					ret, *(buf.i), buf.s, buf.a[0], buf.a[1]);
	}

	return rc;
}

static
int rtti_int8_array_encode_t(void)
{
	const rtti_t int8_type = RTTI_INT_TYPE(int8_t, PRIi8, 1);
	const rtti_t int8_array_type = RTTI_ARRAY_TYPE(&int8_type, 2);
	int i, ret, rc = 0;
	char *buf;

	struct test {
		int8_t n[2];
		const char *s;
		int ret;
	} T[] = {
		{ { 0,  0 }, "[0,0]",  5 },
		{ { 2, -3 }, "[2,-3]", 6 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		ret = rtti_encode(&int8_array_type, NULL, &T[i].n, &buf);

		if (ret  != T[i].ret || !str_equal(buf, T[i].s))
			rc += log_error("[%s/%02d] E[%d,%s] R[%d,%s]",
					__FUNCTION__, i, T[i].ret, T[i].s, ret, buf);

		if (buf)
			mem_free(buf);
	}

	return rc;
}

static
int rtti_int8_array_decode_t(void)
{
	const rtti_t int8_type = RTTI_INT_TYPE(int8_t, PRIi8, 1);
	const rtti_t int8_array_type = RTTI_ARRAY_TYPE(&int8_type, 2);
	int i, ret, rc = 0;
	int8_t buf[2];

	struct test {
		int8_t n[2];
		const char *s;
		int ret;
		int e;
	} T[] = {
		{ { 0,  1 }, "[0,1]",  5, 0 },
		{ { 2, -3 }, "[2,-3]", 6, 0 },
		{ { 2,  0 }, "[2]",    3, 0 },
		{ { 0,  0 }, "2",      0, EINVAL },
		{ { 2,  0 }, "[2 3",   3, EINVAL },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		errno = 0;
		ret = rtti_decode(&int8_array_type, NULL, T[i].s, &buf);

		if (ret != T[i].ret || T[i].n[0] != buf[0] || T[i].n[1] != buf[1] || errno != T[i].e)
			rc += log_error("[%s/%02d] E[%d,{%"PRIi8",%"PRIi8"},%d] R[%d,{%"PRIi8",%"PRIi8"},%d]",
					__FUNCTION__, i,
					T[i].ret, T[i].n[0], T[i].n[1], T[i].e,
					ret, buf[0], buf[1], errno);
	}

	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;

	log_options_t log_options = {
		.log_ident = "rtti",
		.log_dest  = LOGD_STDERR,
		.log_opts  = LOGO_PRIO|LOGO_IDENT,
	};

	log_init(&log_options);

	rc += rtti_bool32_encode_t();
	rc += rtti_bool32_decode_t();
	rc += rtti_int8_encode_t();
	rc += rtti_int8_decode_t();
	rc += rtti_int8_array_encode_t();
	rc += rtti_int8_array_decode_t();
	rc += rtti_struct_encode_t();
	rc += rtti_struct_decode_t();
	rc += rtti_flist_encode_t();
	rc += rtti_flist_decode_t();

	log_close();

	return rc;
}
