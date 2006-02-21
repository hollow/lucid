// Copyright 2005 Peter A. Friend <pafriend@octavian.org>
//           2006 Benedikt Böhm <hollow@gentoo.org>
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

#ifndef _LUCID_TST_H
#define _LUCID_TST_H

#include <sys/types.h>

typedef struct __tst_node {
	char value;
	struct __tst_node *left;
	struct __tst_node *middle;
	struct __tst_node *right;
} _tst_node;

typedef struct __tst_line {
	struct __tst_node *node;
	struct __tst_line *next;
} _tst_line;

typedef struct {
	int lsize;
	_tst_line *lines;
	_tst_node *free;
	_tst_node *head[127];
} TST;

TST  *tst_init   (size_t lsize);
int   tst_insert (TST *tst, char *key, void *data, int replace, void **dup_ptr);
void *tst_search (TST *tst, char *key);
void *tst_delete (TST *tst, char *key);
void  tst_cleanup(TST *tst);

#endif
