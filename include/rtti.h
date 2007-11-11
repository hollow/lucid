// Copyright (c) 2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _LUCID_RTTI_H
#define _LUCID_RTTI_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

/* base definitions */
#define RTTI_SEPARATOR '.'

typedef struct rtti_s rtti_t;

typedef void rtti_init_t(const rtti_t *type, void *data);
typedef void rtti_copy_t(const rtti_t *type, const void *src, void *dst);
typedef bool rtti_equal_t(const rtti_t *type, const void *a, const void *b);
typedef char *rtti_encode_t(const rtti_t *type, const void *data);
typedef void rtti_decode_t(const rtti_t *type, const char **buf, void *data);
typedef void rtti_uninit_t(const rtti_t *type, void *data);

typedef enum {
	RTTI_TYPE_ARRAY,
	RTTI_TYPE_LIST,
	RTTI_TYPE_POINTER,
	RTTI_TYPE_PRIMITIVE,
	RTTI_TYPE_STRUCT,
} rtti_class_t;

struct rtti_s {
	size_t size;
	const char *name;
	rtti_class_t tclass;

	rtti_init_t *init;
	rtti_copy_t *copy;
	rtti_equal_t *equal;
	rtti_encode_t *encode;
	rtti_decode_t *decode;
	rtti_uninit_t *uninit;

	union {
		const void *v;
		const char *s;
		int i;
	} args[3];
};

void rtti_init(const rtti_t *type, const char *name, void *data);
void rtti_free(const rtti_t *type, const char *name, void *data);

void rtti_get(const rtti_t *type, const char *name,
		const void *src, void *dst);
void rtti_set(const rtti_t *type, const char *name,
		const void *src, void *dst);

char *rtti_encode(const rtti_t *type, const char *name,
		const void *data);
void rtti_decode(const rtti_t *type, const char *name,
		const char **buf, void *data);

bool rtti_equal(const rtti_t *type, const char *name,
		const void *data1, const void *data2);

const rtti_t *rtti_find(const rtti_t *type, const char *name, void **datap);

void rtti_get_parser_offset(const char *orig, const char *parsed,
		int *_line, int *_column);

char *rtti_string_parse(const char **str);

/* generic memory region type functions */
extern rtti_init_t   rtti_region_init;
extern rtti_copy_t   rtti_region_copy;
extern rtti_equal_t  rtti_region_equal;

/* type does not support this method */
extern rtti_init_t   rtti_notsup_init;
extern rtti_copy_t   rtti_notsup_copy;
extern rtti_equal_t  rtti_notsup_equal;
extern rtti_encode_t rtti_notsup_encode;
extern rtti_decode_t rtti_notsup_decode;

/* nothing to free for these types */
extern rtti_uninit_t rtti_nothing_free;

/* array types */
#define RTTI_ARRAY_TYPE(etype, asize) { \
	((etype)->size) * (asize), \
	"array", \
	RTTI_TYPE_ARRAY, \
	rtti_array_init, \
	rtti_array_copy, \
	rtti_array_equal, \
	rtti_array_encode, \
	rtti_array_decode, \
	rtti_array_free, \
	{ { (void *)(etype) }, { (void *)(asize) }, { NULL } } \
}

extern rtti_init_t   rtti_array_init;
extern rtti_copy_t   rtti_array_copy;
extern rtti_equal_t  rtti_array_equal;
extern rtti_encode_t rtti_array_encode;
extern rtti_decode_t rtti_array_decode;
extern rtti_uninit_t rtti_array_free;

/* boolean types */
#define RTTI_BOOL_TYPE(type) { \
	sizeof(type), \
	"bool", \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_region_copy, \
	rtti_bool_equal, \
	rtti_bool_encode, \
	rtti_bool_decode, \
	rtti_nothing_free, \
	{ { NULL }, { NULL }, { NULL } } \
}

extern rtti_equal_t  rtti_bool_equal;
extern rtti_encode_t rtti_bool_encode;
extern rtti_decode_t rtti_bool_decode;

extern rtti_t rtti_bool_type;

/* binary data types */
typedef struct rtti_data_s {
	size_t length;
	void *data;
} rtti_data_t;

#define RTTI_DATA_TYPE { \
	sizeof(rtti_data_t), \
	"data", \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_data_copy, \
	rtti_data_equal, \
	rtti_data_encode, \
	rtti_data_decode, \
	rtti_data_free, \
	{ { NULL }, { NULL }, { NULL } } \
}

extern rtti_copy_t   rtti_data_copy;
extern rtti_equal_t  rtti_data_equal;
extern rtti_encode_t rtti_data_encode;
extern rtti_decode_t rtti_data_decode;
extern rtti_uninit_t rtti_data_free;

extern rtti_t rtti_data_type;

/* flist type */
#define RTTI_FLIST_TYPE(size, list, delim, clmod) { \
	sizeof(flag ## size ## _t), \
	"flist" #size, \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_region_copy, \
	rtti_region_equal, \
	rtti_flist ## size ## _encode, \
	rtti_flist ## size ## _decode, \
	rtti_nothing_free, \
	{ { (void *)(list) }, { (void *)(delim) }, { (void *)(clmod) } } \
}

extern rtti_encode_t rtti_flist32_encode;
extern rtti_decode_t rtti_flist32_decode;
extern rtti_encode_t rtti_flist64_encode;
extern rtti_decode_t rtti_flist64_decode;

/* floating point type */
#define RTTI_FLOAT_TYPE(type) { \
	sizeof(type), \
	#type, \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_region_copy, \
	rtti_float_equal, \
	rtti_float_encode, \
	rtti_float_decode, \
	rtti_nothing_free, \
	{ { NULL }, { NULL }, { NULL } } \
}

extern rtti_equal_t  rtti_float_equal;
extern rtti_encode_t rtti_float_encode;
extern rtti_decode_t rtti_float_decode;

extern rtti_t rtti_float_type;
extern rtti_t rtti_double_type;
extern rtti_t rtti_ldouble_type;

/* integer types */
#define RTTI_INT_TYPE(type, sign) { \
	sizeof(type), \
	#type, \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_region_copy, \
	rtti_region_equal, \
	rtti_int_encode, \
	rtti_int_decode, \
	rtti_nothing_free, \
	{ { (void *)(sign) }, { NULL }, { NULL } } \
}

extern rtti_encode_t rtti_int_encode;
extern rtti_decode_t rtti_int_decode;

extern rtti_t rtti_int8_type;
extern rtti_t rtti_uint8_type;
extern rtti_t rtti_int16_type;
extern rtti_t rtti_uint16_type;
extern rtti_t rtti_int32_type;
extern rtti_t rtti_uint32_type;
extern rtti_t rtti_int64_type;
extern rtti_t rtti_uint64_type;

/* pointer types */
#define RTTI_POINTER_TYPE(type) { \
	sizeof(void *), \
	"pointer", \
	RTTI_TYPE_POINTER, \
	rtti_pointer_init, \
	rtti_pointer_copy, \
	rtti_pointer_equal, \
	rtti_pointer_encode, \
	rtti_pointer_decode, \
	rtti_pointer_free, \
	{ { (void *)(type) }, { NULL }, { NULL } } \
}

extern rtti_init_t   rtti_pointer_init;
extern rtti_copy_t   rtti_pointer_copy;
extern rtti_equal_t  rtti_pointer_equal;
extern rtti_encode_t rtti_pointer_encode;
extern rtti_decode_t rtti_pointer_decode;
extern rtti_uninit_t rtti_pointer_free;

extern rtti_t rtti_bool_ptype;
extern rtti_t rtti_data_ptype;
extern rtti_t rtti_float_ptype;
extern rtti_t rtti_double_ptype;
extern rtti_t rtti_ldouble_ptype;
extern rtti_t rtti_int8_ptype;
extern rtti_t rtti_uint8_ptype;
extern rtti_t rtti_int16_ptype;
extern rtti_t rtti_uint16_ptype;
extern rtti_t rtti_int32_ptype;
extern rtti_t rtti_uint32_ptype;
extern rtti_t rtti_int64_ptype;
extern rtti_t rtti_uint64_ptype;
extern rtti_t rtti_string_ptype;

/* string types */
#define RTTI_STRING_TYPE(asnull) { \
	sizeof(char *), \
	"string", \
	RTTI_TYPE_PRIMITIVE, \
	rtti_string_init, \
	rtti_string_copy, \
	rtti_string_equal, \
	rtti_string_encode, \
	rtti_string_decode, \
	rtti_string_free, \
	{ { (void *)(asnull) }, { NULL }, { NULL } } \
}

extern rtti_init_t   rtti_string_init;
extern rtti_copy_t   rtti_string_copy;
extern rtti_equal_t  rtti_string_equal;
extern rtti_encode_t rtti_string_encode;
extern rtti_decode_t rtti_string_decode;
extern rtti_uninit_t rtti_string_free;

extern rtti_t rtti_string_type;

/* struct types */
typedef struct rtti_field_s {
	const char *name;
	const rtti_t *type;
	size_t size;
	off_t offset;
} rtti_field_t;

#define RTTI_STRUCT_FIELD_START(name) \
	const rtti_field_t __ ## name ## _fields[] = {

#define RTTI_STRUCT_FIELD(sname, fname, ftype) \
		{ #fname, ftype, sizeof(((struct sname *)0)->fname), \
			offsetof(struct sname, fname) },

#define RTTI_STRUCT_FIELD2(sname, fname, dname, ftype) \
		{ #dname, ftype, sizeof(((struct sname *)0)->fname), \
			offsetof(struct sname, fname) },

#define RTTI_STRUCT_FIELD_END \
		{ NULL, NULL, 0, 0 } \
	};

#define RTTI_STRUCT_TYPE(name) { \
	sizeof(struct name), \
	"struct " #name, \
	RTTI_TYPE_STRUCT, \
	rtti_struct_init, \
	rtti_struct_copy, \
	rtti_struct_equal, \
	rtti_struct_encode, \
	rtti_struct_decode, \
	rtti_struct_free, \
	{ { (void *)(__ ## name ## _fields) }, { NULL }, { NULL } } \
}

extern rtti_init_t   rtti_struct_init;
extern rtti_copy_t   rtti_struct_copy;
extern rtti_equal_t  rtti_struct_equal;
extern rtti_encode_t rtti_struct_encode;
extern rtti_decode_t rtti_struct_decode;
extern rtti_uninit_t rtti_struct_free;

/* list types */
#define RTTI_LIST_TYPE(lname, loff, flist) { \
	sizeof(struct lname), \
	"list " #lname, \
	RTTI_TYPE_LIST, \
	rtti_list_init, \
	rtti_list_copy, \
	rtti_notsup_equal, \
	rtti_list_encode, \
	rtti_list_decode, \
	rtti_list_free, \
	{ { (void *)(flist) }, { (void *)(loff) }, { NULL } } \
}

extern rtti_init_t   rtti_list_init;
extern rtti_copy_t   rtti_list_copy;
extern rtti_encode_t rtti_list_encode;
extern rtti_decode_t rtti_list_decode;
extern rtti_uninit_t rtti_list_free;

void rtti_list_add(const rtti_t *type, void *entry, void *head);
void rtti_list_del(const rtti_t *type, void *entry);
void rtti_list_move(const rtti_t *type, void *entry, void *head);
int  rtti_list_empty(const rtti_t *type, void *head);
void rtti_list_splice(const rtti_t *type, void *list, void *head);

/* various other types */
extern rtti_encode_t rtti_inaddr_encode;
extern rtti_decode_t rtti_inaddr_decode;
extern rtti_t rtti_inaddr_type;

#endif
