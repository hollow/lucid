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

void *tst_delete(TST *tst, char *key)
{
	_tst_node *cur;
	_tst_node *curp; /* parent */
	_tst_node *last;
	_tst_node *lastp; /* parent */
	_tst_node *lastr; /* replacement */
	_tst_node *lastd; /* dangling child */
	_tst_node *next;
	
	int idx;
	
	if (key == NULL) {
		errno = EINVAL;
		return NULL;
	}
	
	if (key[0] == 0) {
		errno = EINVAL;
		return NULL;
	}
	
	idx = (int) key[0];
	cur = tst->head[idx];
	
	if (cur == NULL)
		return NULL;
	
	last = NULL;
	lastp = NULL;
	curp = NULL;
	idx = 1;
	
	while(cur != NULL) {
		if(key[idx] == cur->value) {
			if ((cur->left != NULL) || (cur->right != NULL)) {
				last = cur;
				lastp = curp;
			}
			
			if (key[idx] == 0)
				break;
			
			else {
				curp = cur;
				cur = cur->middle;
				idx++;
				continue;
			}
		}
		
		else if (((cur->value == 0) && (key[idx] < 64)) ||
		         ((cur->value != 0) && (key[idx] < cur->value))) {
			lastp = cur;
			curp = cur;
			cur = cur->left;
			last = cur;
			continue;
		}
		
		else {
			lastp = cur;
			curp = cur;
			cur = cur->right;
			last = cur;
			continue;
		}
	}
	
	if (cur == NULL)
		return NULL;
	
	idx = (int) key[0];
	
	if (last == NULL) {
		next = tst->head[idx];
		tst->head[idx] = NULL;
	}
	
	else if ((last->left == NULL) && (last->right == NULL)) {
		if(lastp->left == last)
			lastp->left = NULL;
		else
			lastp->right = NULL;
		
		next = last;
	}
	
	else {
		if((last->left != NULL) && (last->right != NULL)) {
			lastr = last->right;
			lastd = last->left;
		}
		
		else if (last->right != NULL) {
			lastr = last->right;
			lastd = NULL;
		}
		
		else {
			lastr = last->left;
			lastd = NULL;
		}
			
		if (lastp == NULL)
			tst->head[idx] = lastr;
		else {
			if (lastp->left == last)
				lastp->left = lastr;
			else if (lastp->right == last)
				lastp->right = lastr;
			else
				lastp->middle = lastr;
		}
		
		if (lastd != NULL) {
			cur = lastr;
			
			while (cur->left != NULL)
				cur = cur->left;
			
			cur->left = lastd;
		}
		
		next = last;
	}
	
	do {
		cur = next;
		next = cur->middle;
		
		cur->left = NULL;
		cur->right = NULL;
		cur->middle = tst->free;
		tst->free = cur;
	} while(cur->value != 0);
	
	return next;
}
