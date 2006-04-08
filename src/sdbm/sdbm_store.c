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

int sdbm_store(SDBM *db, DATUM key, DATUM val, int flags)
{
	int need;
	long hash;
	
	if (db == NULL || bad(key))
		return errno = EINVAL, -1;
	
	if (sdbm_rdonly(db))
		return errno = EPERM, -1;
	
	need = key.dsize + val.dsize;
	
	if (need < 0 || need > SDBM_PAIRMAX)
		return errno = EINVAL, -1;
	
	if (getpage(db, (hash = exhash(key)))) {
		if (flags == SDBM_REPLACE)
			delpair(db->pagbuf, key);
		
		if (!fitpair(db->pagbuf, need))
			if (!makroom(db, hash, need))
				return ioerr(db), -1;
		
		putpair(db->pagbuf, key, val);
		
		if (lseek(db->pagf, OFF_PAG(db->pagbno), SEEK_SET) < 0 ||
		    write(db->pagf, db->pagbuf, SDBM_PBLKSIZ) < 0)
			return ioerr(db), -1;
		
		return 0;
	}
	
	return ioerr(db), -1;
}
