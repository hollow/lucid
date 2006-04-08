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
#include <stdio.h>
#include <strings.h>
#include <sys/fcntl.h>

#define SDBM_INTERNAL
#include "sdbm.h"

SDBM *sdbm_open(char *file, int flags, int mode)
{
	int errno_orig;
	SDBM *db;
	char *dirname, *pagname;
	struct stat sb;
	
	if (file == NULL || !file[0])
		return errno = EINVAL, NULL;
	
	asprintf(&dirname, "%s%s", file, SDBM_DIRFEXT);
	asprintf(&pagname, "%s%s", file, SDBM_PAGFEXT);
	
	if ((db = malloc(sizeof(SDBM))) == NULL)
		return errno = ENOMEM, NULL;
	else
		bzero(db, sizeof(SDBM));
	
	if (flags & O_WRONLY)
		flags = (flags & ~O_WRONLY) | O_RDWR;
	else if ((flags & 03) == O_RDONLY)
		db->flags = SDBM_RDONLY;
	
	if ((db->pagf = open(pagname, flags, mode)) == -1)
		goto err;
	
	if (sdbm_lock(db->pagf, sdbm_rdonly(db)) == -1 )
		goto err;
	
	if ((db->dirf = open(dirname, flags, mode)) == -1)
		goto err;
	
	if (fstat(db->dirf, &sb) == -1)
		goto err;
	
	db->dirbno = (!sb.st_size) ? 0 : -1;
	db->pagbno = -1;
	db->maxbno = sb.st_size * BYTESIZ;
	
	bzero(db->pagbuf, SDBM_PBLKSIZ);
	bzero(db->dirbuf, SDBM_DBLKSIZ);
	
	return db;
	
err:
	errno_orig = errno;
	
	if (db->dirf > 0)
		close(db->dirf);
	
	if (db->pagf > 0) {
		sdbm_unlock(db->pagf);
		close(db->pagf);
	}
	
	free(db);
	db = NULL;
	
	errno = errno_orig;

out:
	free(dirname);
	free(pagname);
	return db;
}
