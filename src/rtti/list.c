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

#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "cext.h"
#include "list.h"
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

#include "internal.h"

#define LISTP(T, D) ((void *)((char *)(D) + (T)->args[2].i))
#define LISTC(T, D) ((void *)((char *)(D) - (T)->args[2].i))

#define MEMBP(F, D) ((void *)((char *)(D) + (F)->offset))

void rtti_list_init(const rtti_t *type, void *data)
{
	CHECK_TYPE(LIST);

	memset(data, 0, type->size);

	const rtti_field_t *field;
	for (field = type->args[0].v; field->name != NULL; field++) {
		void *memb = MEMBP(field, data);
		field->type->init(field->type, memb);
		error_dof("failed to initialize list (%s)", field->name)
			return;
	}

	list_t *list = LISTP(type, data);
	INIT_LIST_HEAD(list);
}

void rtti_list_copy(const rtti_t *type, const void *src, void *dst)
{
	CHECK_TYPE(LIST);

	rtti_list_init(type, dst);

	const list_t *shead = LISTP(type, src);
	const list_t *pos = shead;

	list_for_each(pos, shead) {
		src = LISTC(type, pos);

		void *entry = malloc(type->size);
		type->init(type, entry);
		error_dof("failed to initialize list node")
			return;

		const rtti_field_t *field;
		for (field = type->args[0].v; field->name != NULL; field++) {
			const void *smemb = MEMBP(field, src);
			void *dmemb = MEMBP(field, entry);

			field->type->copy(field->type, smemb, dmemb);
			error_dof("failed to copy list node (%s)", field->name)
				break;
		}

		error_dof("failed to copy list (%s)", type->name)
			return;

		rtti_list_add(type, entry, dst);
	}
}

char *rtti_list_encode(const rtti_t *type, const void *data)
{
	CHECK_TYPE(LIST);

	stralloc_t _buf, *buf = &_buf;
	stralloc_init(buf);
	stralloc_cats(buf, "[");

	const list_t *head = LISTP(type, data);
	const list_t *pos;

	list_for_each(pos, head) {
		data = LISTC(type, pos);

		stralloc_cats(buf, "{");

		const rtti_field_t *field;
		for (field = type->args[0].v; field->name != NULL; field++) {
			const rtti_t *ktype = &rtti_string_type;
			void *memb = MEMBP(field, data);

			char *kbuf = ktype->encode(ktype, &field->name);
			error_dof("failed to encode list node (%s)", field->name)
				return NULL;

			char *vbuf = field->type->encode(field->type, memb);
			error_dof("failed to encode list node (%s)", field->name)
				return NULL;

			stralloc_catm(buf, kbuf, ":", vbuf, NULL);

			if ((field + 1)->name != NULL)
				stralloc_cats(buf, ",");
		}

		stralloc_cats(buf, "}");

		if (pos->next != head)
			stralloc_cats(buf, ",");
	}

	stralloc_cats(buf, "]");
	return stralloc_finalize(buf);
}

void rtti_list_decode(const rtti_t *type, const char **buf, void *data)
{
	CHECK_TYPE(LIST);

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, '[') {
		error_set(EILSEQ, "expected LSQBR near '%.16s'", *buf);
		return;
	}

	while (1) {
		SKIP_SPACE(buf);
		SKIP_CHAR(buf, '{') {
			error_set(EILSEQ, "expected LCUBR near '%.16s'", *buf);
			return;
		}

		void *entry = malloc(type->size);
		type->init(type, entry);
		error_dof("failed to initialize list node")
			return;

		while (1) {
			const rtti_t *ktype = &rtti_string_type;
			const char *kbuf;
			ktype->decode(ktype, buf, &kbuf);
			error_dof("failed to decode list node key")
				break;

			const rtti_field_t *field;
			for (field = type->args[0].v; field->name != NULL; field++) {
				if (str_equal(field->name, kbuf))
					break;
			}

			if (field->name == NULL) {
				error_set(ENOENT, "unknown list node member (%s)", kbuf);
				break;
			}

			SKIP_SPACE(buf);
			SKIP_CHAR(buf, ':') {
				error_set(EILSEQ, "expected COLON near '%.16s'", *buf);
				break;
			}

			void *memb = MEMBP(field, data);
			field->type->decode(field->type, buf, memb);
			error_dof("failed to decode list node member (%s)", kbuf)
				break;

			SKIP_SPACE(buf);
			SKIP_CHAR(buf, ',') {
				break;
			}
		}

		error_dof("failed to decode list node (%s)", type->name)
			return;

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, '}') {
			error_set(EILSEQ, "expected RCUBR near '%.16s'", *buf);
			return;
		}

		rtti_list_add(type, entry, data);

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, ',') {
			break;
		}
	}

	SKIP_SPACE(buf);
	SKIP_CHAR(buf, ']') {
		error_set(EILSEQ, "expected RSQBR near '%.16s'", *buf);
		return;
	}
}

void rtti_list_add(const rtti_t *type, void *entry, void *head)
{
	const int tail = type->args[1].i;

	if (tail)
		list_add_tail(entry, head);
	else
		list_add(entry, head);
}

size_t rtti_list_length(const rtti_t *type, const void *data)
{
	size_t length = 0;
	list_t *head = LISTP(type, data);
	list_t *pos;

	list_for_each(pos, head)
		length++;

	return length;
}
