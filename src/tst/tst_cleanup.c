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

void tst_cleanup(TST *tst)
{
	_tst_line *cur;
	_tst_line *next;
	
	next = tst->lines;
	
	do {
		cur = next;
		next = cur->next;
		free(cur->node);
		free(cur);
	} while (next != NULL);
	
	free(tst);
}
