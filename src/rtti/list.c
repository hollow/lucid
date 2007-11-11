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

#include "error.h"
#include "mem.h"
#include "list.h"
#include "rtti.h"
#include "str.h"
#include "stralloc.h"

#include "internal.h"

#define IDX(D, F) ((void *)((char *)(D) + (F)->offset))
#define ODX(L, F) ((void *)((char *)(L) - (F)->offset))

void rtti_list_init(const rtti_t *type, void *data)
{
	const rtti_field_t *field;
	list_t *list = data;

	CHECK_TYPE(LIST);

	mem_set(data, 0, type->size);

	for (field = type->args[0].v; field->name != NULL; field++) {
		field->type->init(field->type, IDX(data, field));
		error_dof("failed to initialize list (%s)", field->name) {
			while (field-- != type->args[0].v)
				field->type->uninit(field->type, IDX(data, field));
			mem_set(data, 0, type->size);
			return;
		}
	}

	INIT_LIST_HEAD(list);
}

void rtti_list_copy(const rtti_t *type, const void *src, void *dst)
{
	const rtti_field_t *field;
	const list_t *shead = src, *pos = shead;

	CHECK_TYPE(LIST);

	while ((pos = rtti_list_next(type, pos)) != shead) {
		src = pos;

		void *entry = mem_alloc(type->size);
		type->init(type, entry);
		error_dof("failed to initialize list node") {
			mem_free(entry);
			rtti_list_free(type, dst);
			return;
		}

		for (field = type->args[0].v; field->name != NULL; field++) {
			const void *const sdata = IDX(src, field);
			void *const ddata = IDX(entry, field);

			field->type->copy(field->type, sdata, ddata);
			error_dof("failed to copy list node (%s)", field->name) {
				mem_free(entry);
				break;
			}
		}

		error_dof("failed to copy list (%s)", type->name) {
			rtti_list_free(type, dst);
			return;
		}

		rtti_list_add(type, entry, dst);
	}
}

char *rtti_list_encode(const rtti_t *type, const void *data)
{
	const rtti_t *ftype, *ktype = &rtti_string_type;
	const rtti_field_t *field;
	const list_t *head = data, *pos = head;
	stralloc_t _rbuf, *rbuf = &_rbuf;
	char *fbuf, *buf = NULL;

	CHECK_TYPE(LIST);

	stralloc_init(rbuf);
	stralloc_cats(rbuf, "[");

	while ((pos = rtti_list_next(type, pos)) != head) {
		data = pos;

		stralloc_cats(rbuf, "{");

		for (field = type->args[0].v; field->name != NULL; field++) {
			ftype = field->type;

			fbuf = ktype->encode(ktype, &field->name);
			error_dof("failed to encode list node (%s)", field->name)
				goto out;

			stralloc_catm(rbuf, fbuf, ":", NULL);
			mem_free(fbuf);

			fbuf = field->type->encode(ftype, IDX(data, field));
			error_dof("failed to encode list node (%s)", field->name)
				goto out;

			stralloc_cats(rbuf, fbuf);
			mem_free(fbuf);

			if ((field + 1)->name != NULL)
				stralloc_cats(rbuf, ",");
		}

		stralloc_cats(rbuf, "}");

		if (rtti_list_next(type, pos) != head)
			stralloc_cats(rbuf, ",");
	}

	stralloc_cats(rbuf, "]");
	buf = stralloc_finalize(rbuf);

out:
	stralloc_free(rbuf);
	return buf;
}

void rtti_list_decode(const rtti_t *type, const char **buf, void *data)
{
	const rtti_t *ftype, *ktype = &rtti_string_type;
	void *head = data, *fdata;
	const char *key;

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

		data = mem_alloc(type->size);

		while (1) {
			ktype->decode(ktype, buf, &key);
			error_dof("failed to decode list node key")
				break;

			fdata = data;

			ftype = rtti_find(type, key, &fdata);
			error_do {
				ktype->uninit(ktype, &key);
				break;
			}

			SKIP_SPACE(buf);
			SKIP_CHAR(buf, ':') {
				error_set(EILSEQ, "expected COLON near '%.16s'", *buf);
				ktype->uninit(ktype, &key);
				break;
			}

			ftype->decode(ftype, buf, fdata);
			error_dof("failed to decode list node member %s (%s)", key, ftype->name) {
				ktype->uninit(ktype, &key);
				break;
			}

			ktype->uninit(ktype, &key);

			SKIP_SPACE(buf);
			SKIP_CHAR(buf, ',') {
				break;
			}
		}

		error_dof("failed to decode list node") {
			mem_free(data);
			return;
		}

		SKIP_SPACE(buf);
		SKIP_CHAR(buf, '}') {
			error_set(EILSEQ, "expected RCUBR near '%.16s'", *buf);
			return;
		}

		rtti_list_add(type, data, head);

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

void rtti_list_free(const rtti_t *type, void *data)
{
	const rtti_field_t *field;
	list_t *pos, *tmp, *head = data;

	CHECK_TYPE(LIST);

	pos = rtti_list_next(type, head);
	tmp = rtti_list_next(type, pos);

	while (pos != head) {
		data = pos;

		for (field = type->args[0].v; field->name != NULL; field++)
			field->type->uninit(field->type, IDX(data, field));

		list_del(data);
		mem_set(data, 0, type->size);
		mem_free(data);

		pos = tmp;
		tmp = rtti_list_next(type, pos);
	}
}

list_t *rtti_list_prev(const rtti_t *type, const void *_entry)
{
	const int tail = type->args[1].i;
	const list_t *entry = _entry;

	if (tail)
		return entry->prev;
	else
		return entry->next;
}

list_t *rtti_list_next(const rtti_t *type, const void *_entry)
{
	const int tail = type->args[1].i;
	const list_t *entry = _entry;

	if (tail)
		return entry->next;
	else
		return entry->prev;
}

void rtti_list_add(const rtti_t *type, void *entry, void *head)
{
	const int tail = type->args[1].i;

	if (tail)
		list_add_tail(entry, head);
	else
		list_add(entry, head);
}
