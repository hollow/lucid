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

void *tst_search(TST *tst, char *key)
{
	_tst_node *cur;
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
	
	if((cur = tst->head[idx]) == NULL)
		return NULL;
	
	idx = 1;
	
	while (cur != NULL) {
		if (key[idx] == cur->value) {
			if (cur->value == 0)
				return cur->middle;
			else {
				cur = cur->middle;
				idx++;
				continue;
			}
		}
		
		else if (((cur->value == 0) && (key[idx] < 64)) ||
		         ((cur->value != 0) && (key[idx] < cur->value))) {
			cur = cur->left;
			continue;
		}
		
		else {
			cur = cur->right;
			continue;
		}
	}
	
	return NULL;
}

