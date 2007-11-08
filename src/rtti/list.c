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
	const rtti_field_t *field, *listf = type->args[1].v;
	list_t *list = IDX(data, listf);

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
	const rtti_field_t *field, *listf = type->args[1].v;
	const int tail = type->args[2].i;
	list_t *pos, *shead = IDX(src, listf);
	void *entry;

	CHECK_TYPE(LIST);

	/* ugly foreach hack to handle args->tail */
	pos = tail ? shead->next : shead->prev;

	while (pos != shead) {
		src = ODX(pos, listf);

		entry = mem_alloc(type->size);

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
		pos = tail ? pos->next : pos->prev;
	}
}

char *rtti_list_encode(const rtti_t *type, const void *data)
{
	const rtti_t *ftype, *ktype = &rtti_string_type;
	const int tail = type->args[2].i;
	const rtti_field_t *field, *listf = type->args[1].v;
	list_t *pos, *head = IDX(data, listf);
	stralloc_t _rbuf, *rbuf = &_rbuf;
	char *fbuf, *buf = NULL;

	CHECK_TYPE(LIST);

	stralloc_init(rbuf);
	stralloc_cats(rbuf, "[");

	/* ugly foreach hack to handle args->tail */
	pos = tail ? head->next : head->prev;

	while (pos != head) {
		data = ODX(pos, listf);

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

		pos = tail ? pos->next : pos->prev;

		if (pos != head)
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
			error_dof("failed to decode list node near '%.16s'", *buf)
				return;

			fdata = data;

			ftype = rtti_find(type, key, &fdata);
			error_dof("list member does not exist (%s)", key)
				return;

			ktype->uninit(ktype, &key);

			SKIP_SPACE(buf);
			SKIP_CHAR(buf, ':') {
				error_set(EILSEQ, "expected COLON near '%.16s'", *buf);
				return;
			}

			ftype->decode(ftype, buf, fdata);
			error_dof("failed to decode list node (%s) near '%.16s'", ftype->name, *buf)
				return;

			SKIP_SPACE(buf);
			SKIP_CHAR(buf, ',') {
				break;
			}
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
	const int tail = type->args[2].i;
	const rtti_field_t *field, *listf = type->args[1].v;
	list_t *pos, *tmp, *head = IDX(data, listf);

	CHECK_TYPE(LIST);

	/* ugly foreach hack to handle args->tail */
	pos = tail ? head->next : head->prev;
	tmp = tail ? pos->next : pos->next;

	while (pos != head) {
		data = ODX(pos, listf);

		for (field = type->args[0].v; field->name != NULL; field++)
			field->type->uninit(field->type, IDX(data, field));

		rtti_list_del(type, data);
		mem_set(data, 0, type->size);
		mem_free(data);

		pos = tmp;
		tmp = tail ? pos->next : pos->prev;
	}
}

void rtti_list_add(const rtti_t *type, void *_entry, void *_head)
{
	const rtti_field_t *listf = type->args[1].v;
	const int tail = type->args[2].i;
	list_t *entry = IDX(_entry, listf);
	list_t *head = IDX(_head, listf);

	if (tail)
		list_add_tail(entry, head);
	else
		list_add(entry, head);
}

void rtti_list_del(const rtti_t *type, void *_entry)
{
	const rtti_field_t *listf = type->args[1].v;
	list_t *entry = IDX(_entry, listf);
	list_del(entry);
}

void rtti_list_move(const rtti_t *type, void *_entry, void *_head)
{
	const rtti_field_t *listf = type->args[1].v;
	list_t *entry = IDX(_entry, listf);
	list_t *head  = IDX(_head,  listf);
	list_move(entry, head);
}

int rtti_list_empty(const rtti_t *type, void *_head)
{
	const rtti_field_t *listf = type->args[1].v;
	list_t *head = IDX(_head, listf);
	return list_empty(head);
}

void rtti_list_splice(const rtti_t *type, void *_list, void *_head)
{
	const rtti_field_t *listf = type->args[1].v;
	list_t *list = IDX(_list, listf);
	list_t *head = IDX(_head, listf);
	list_splice(list, head);
}
