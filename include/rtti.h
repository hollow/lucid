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
#include <sys/types.h>

/* base definitions */
#define RTTI_SEPARATOR '.'

typedef struct rtti_s rtti_t;

typedef int rtti_init_t(const rtti_t *type, void *data);
typedef int rtti_copy_t(const rtti_t *type, const void *src, void *dst);
typedef int rtti_equal_t(const rtti_t *type, const void *a, const void *b);
typedef int rtti_encode_t(const rtti_t *type, const void *data, char **buf);
typedef int rtti_decode_t(const rtti_t *type, const char *buf, void *data);
typedef int rtti_uninit_t(const rtti_t *type, void *data);

typedef enum {
	RTTI_TYPE_PRIMITIVE,
	RTTI_TYPE_POINTER,
	RTTI_TYPE_ARRAY,
	RTTI_TYPE_VECTOR,
	RTTI_TYPE_STRUCT,
	RTTI_TYPE_UNION,
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

int rtti_init(const rtti_t *type, const char *name, void *data);
int rtti_free(const rtti_t *type, const char *name, void *data);

int rtti_get(const rtti_t *type, const char *name,
		const void *src, void *dst);
int rtti_set(const rtti_t *type, const void *src,
		const char *name, void *dst);

int rtti_encode(const rtti_t *type, const char *name,
		const void *data, char **buf);
int rtti_decode(const rtti_t *type, const char *name,
		const char *buf, void *data);

int rtti_compare(const rtti_t *type, const char *name,
		const void *data1, const void *data2);

const rtti_t *rtti_find(const rtti_t *type, const char *name, void **datap);

int rtti_string_parse(const char *str, char **buf);

/* generic memory region type functions */
extern rtti_init_t   rtti_region_init;
extern rtti_copy_t   rtti_region_copy;
extern rtti_equal_t  rtti_region_equal;

/* type does not support this method */
extern rtti_init_t   rtti_notsupp_init;
extern rtti_copy_t   rtti_notsupp_copy;
extern rtti_equal_t  rtti_notsupp_equal;
extern rtti_encode_t rtti_notsupp_encode;
extern rtti_decode_t rtti_notsupp_decode;

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
	{ { NULL }, { NULL }, { NULL } \
}

extern rtti_copy_t   rtti_data_copy;
extern rtti_equal_t  rtti_data_equal;
extern rtti_encode_t rtti_data_encode;
extern rtti_decode_t rtti_data_decode;
extern rtti_uninit_t rtti_data_free;

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
#define RTTI_FLOAT_TYPE(type, pri_arg, sign) { \
	sizeof(type), \
	#type, \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_region_copy, \
	rtti_float_equal, \
	rtti_float_encode, \
	rtti_float_decode, \
	rtti_nothing_free, \
	{ { (void *)("%" pri_arg) }, { NULL }, { (void *)sign } } \
}

extern rtti_equal_t  rtti_float_equal;
extern rtti_encode_t rtti_float_encode;
extern rtti_decode_t rtti_float_decode;

/* integer types */
#define RTTI_INT_TYPE(type, pri_arg, sign) { \
	sizeof(type), \
	#type, \
	RTTI_TYPE_PRIMITIVE, \
	rtti_region_init, \
	rtti_region_copy, \
	rtti_region_equal, \
	rtti_int_encode, \
	rtti_int_decode, \
	rtti_nothing_free, \
	{ { (void *)("%" pri_arg) }, { NULL }, { (void *)sign } } \
}

extern rtti_encode_t rtti_int_encode;
extern rtti_decode_t rtti_int_decode;

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

/* string types */
#define RTTI_STRING_TYPE(maynull, asnull) { \
	sizeof(char *), \
	"string", \
	RTTI_TYPE_PRIMITIVE, \
	rtti_string_init, \
	rtti_string_copy, \
	rtti_string_equal, \
	rtti_string_encode, \
	rtti_string_decode, \
	rtti_string_free, \
	{ { (void *)(maynull) }, { (void *)(asnull) }, { NULL } } \
}

extern rtti_init_t   rtti_string_init;
extern rtti_copy_t   rtti_string_copy;
extern rtti_equal_t  rtti_string_equal;
extern rtti_encode_t rtti_string_encode;
extern rtti_decode_t rtti_string_decode;
extern rtti_uninit_t rtti_string_free;

/* struct types */
typedef struct rtti_field_s {
	const char *name;
	const rtti_t *type;
	size_t size;
	off_t offset;
} rtti_field_t;

#define RTTI_STRUCT_FIELD(sname, fname, ftype) \
	{ #fname, ftype, sizeof(((struct sname *)0)->fname), \
		offsetof(struct sname, fname) }

#define RTTI_STRUCT_FIELD2(sname, fname, dname, ftype) \
	{ dname, ftype, sizeof(((struct sname *)0)->fname), \
		offsetof(struct sname, fname) }

#define RTTI_STRUCT_FIELD_END { NULL, NULL, 0, 0 }

#define RTTI_STRUCT_TYPE(sname, flist) { \
	sizeof(struct sname), \
	"struct " #sname, \
	RTTI_TYPE_STRUCT, \
	rtti_struct_init, \
	rtti_struct_copy, \
	rtti_struct_equal, \
	rtti_struct_encode, \
	rtti_struct_decode, \
	rtti_struct_free, \
	{ { (void *)(flist) }, { NULL }, { NULL } } \
}

extern rtti_init_t   rtti_struct_init;
extern rtti_copy_t   rtti_struct_copy;
extern rtti_equal_t  rtti_struct_equal;
extern rtti_encode_t rtti_struct_encode;
extern rtti_decode_t rtti_struct_decode;
extern rtti_uninit_t rtti_struct_free;

#endif
