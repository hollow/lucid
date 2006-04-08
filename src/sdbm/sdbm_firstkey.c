// Copyright ???? oz@nexus.yorku.ca
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define SDBM_INTERNAL
#include "sdbm.h"

DATUM sdbm_firstkey(SDBM *db)
{
	if (db == NULL)
		return errno = EINVAL, nullDATUM;
	
	if (lseek(db->pagf, OFF_PAG(0), SEEK_SET) < 0 ||
	    read(db->pagf, db->pagbuf, SDBM_PBLKSIZ) < 0)
		return ioerr(db), nullDATUM;
	
	db->pagbno = 0;
	db->blkptr = 0;
	db->keyptr = 0;
	
	return getnext(db);
}
