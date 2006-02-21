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
#include <errno.h>

#include "tst/tst.h"


static
int _tst_grow(TST *tst)
{
	_tst_node *cur;
	_tst_line *new;
	int i;
	
	/* alloc a new node line */
	new = (_tst_line *) malloc(sizeof(_tst_line));
	
	if (!new)
		return -1;
	
	/* alloc line nodes */
	new->node = (_tst_node *) calloc(tst->lsize, sizeof(_tst_node));
	
	if (!new->node) {
		free(new);
		return -1;
	}
	
	/* link new line */
	new->next = tst->lines;
	tst->lines = new;
	
	/* link new empty nodes */
	tst->free = tst->lines->node;
	
	/* link new nodes to each other */
	cur = tst->lines->node;
	
	for (i = 1; i < tst->lsize; i++) {
		cur->middle = &(tst->lines->node[i]);
		cur = cur->middle;
	}
	
	cur->middle = NULL;
	
	return 0;
}

int tst_insert(TST *tst, char *key, void *data, int replace, void **dup_ptr)
{
	_tst_node *cur;
	_tst_node *new = NULL;
	
	int idx;
	int loop = 1;
	
	if (key == NULL) {
		errno = EINVAL;
		return -1;
	}
	
	if (key[0] == 0) {
		errno = EINVAL;
		return -1;
	}
	
	idx = (int) key[0];
	
	/* head pointer is empty */
	if (tst->head[idx] == NULL) {
		/* no free nodes */
		if (tst->free == NULL && _tst_grow(tst) == -1)
			return -1;
		
		/* link next free node */
		tst->head[idx] = tst->free;
		tst->free = tst->free->middle;
		
		cur = tst->head[idx];
		cur->value = key[1];
		
		if(key[1] == 0) {
			cur->middle = data;
			return 0;
		}
		
		/* no duplicates possible anymore */
		loop = 0;
	}
	
	cur = tst->head[idx];
	idx = 1;
	
	while (loop == 1) {
		/* matching node found */
		if (key[idx] == cur->value) {
			/* duplicate key found */
			if(key[idx] == 0) {
				
				/* replace it */
				if (replace == 1) {
					if (dup_ptr != NULL)
						*dup_ptr = cur->middle;
					
					cur->middle = data;
					return 0;
				}
				
				/* or just return the duplicate key in *dup_ptr */
				else {
					if (dup_ptr != NULL)
						*dup_ptr = cur->middle;
					
					errno = EEXIST;
					return -1;
				}
			}
			
			/* still looking for duplicates */
			else {
				/* no more nodes */
				if(cur->middle == NULL) {
					if (tst->free == NULL && _tst_grow(tst) == -1)
						return -1;
					
					/* insert new node */
					cur->middle = tst->free;
					tst->free = tst->free->middle;
					
					/* set current node as new node */
					new = cur;
					cur = cur->middle;
					cur->value = key[idx];
					
					/* no duplicates possible anymore */
					break;
				}
				
				/* check next node */
				else {
					cur = cur->middle;
					idx++;
					continue;
				}
			}
		}
		
		/* key is less than node value */
		if (((cur->value == 0) && (key[idx] < 64)) ||
		    ((cur->value != 0) && (key[idx] < cur->value))) {
			/* insert new left node */
			if (cur->left == NULL) {
				if (tst->free == NULL && _tst_grow(tst) == -1)
					return -1;
				
				cur->left = tst->free;
				tst->free = tst->free->middle;
				
				new = cur;
				cur = cur->left;
				cur->value = key[idx];
				
				if (key[idx] == 0) {
					cur->middle = data;
					return 0;
				}
				
				else
					break;
			}
			
			else {
				cur = cur->left;
				continue;
			}
		}
		
		/* key is greather than node value */
		else {
			/* insert new right node */
			if (cur->right == NULL) {
				if (tst->free == NULL && _tst_grow(tst) != 1)
					return -1;
				
				cur->right = tst->free;
				tst->free = tst->free->middle;
				
				new = cur;
				cur = cur->right;
				cur->value = key[idx];
				
				break;
			}
			
			else {
				cur = cur->right;
				continue;
			}
		}
	}
	
	do {
		idx++;
		
		/* remove floating nodes after allocation failure */
		if (tst->free == NULL && _tst_grow(tst) == -1) {
			cur = new->middle;
			
			while (cur->middle != NULL)
				cur = cur->middle;
			
			cur->middle = tst->free;
			tst->free = new->middle;
			new->middle = NULL;
			
			return -1;
		}
		
		/* add new free node */
		cur->middle = tst->free;
		tst->free = tst->free->middle;
		
		cur = cur->middle;
		cur->value = key[idx];
	} while(key[idx] != 0);
	
	cur->middle = data;
	
	return 0;
}
