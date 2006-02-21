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

#include <stdlib.h>

#include "tst/tst.h"

TST *tst_init(size_t lsize)
{
	TST *tst;
	
	_tst_node *cur;
	int i;
	
	if (lsize < 1)
		lsize = 1;
	
	/* alloc global TST struct */
	tst = (TST *) calloc(1, sizeof(TST));
	
	if (!tst)
		return NULL;
	
	/* alloc a new node line */
	tst->lines = (_tst_line *) calloc(1, sizeof(_tst_line));
	
	if (!tst->lines) {
		free(tst);
		return NULL;
	}
	
	/* alloc line nodes */
	tst->lsize = lsize;
	tst->lines->next = NULL;
	
	tst->lines->node = (_tst_node *) calloc(lsize, sizeof(_tst_node));
	
	if (!tst->lines->node) {
		free(tst->lines);
		free(tst);
		return NULL;
	}
	
	/* link new empty nodes */
	tst->free = tst->lines->node;
	
	/* link new nodes to each other */
	cur = tst->lines->node;
	
	for (i = 1; i < (int) lsize; i++) {
		cur->middle = &(tst->lines->node[i]);
		cur = cur->middle;
	}
	
	cur->middle = NULL;
	
	return tst;
}
