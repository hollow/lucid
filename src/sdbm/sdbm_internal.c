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

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>

#define SDBM_INTERNAL
#include "sdbm.h"

static long masks[] = {
	000000000000, 000000000001, 000000000003, 000000000007,
	000000000017, 000000000037, 000000000077, 000000000177,
	000000000377, 000000000777, 000000001777, 000000003777,
	000000007777, 000000017777, 000000037777, 000000077777,
	000000177777, 000000377777, 000000777777, 000001777777,
	000003777777, 000007777777, 000017777777, 000037777777,
	000077777777, 000177777777, 000377777777, 000777777777,
	001777777777, 003777777777, 007777777777, 017777777777
};

DATUM nullDATUM = { NULL, 0 };

int seepair(char *pag, int n, char *key, int siz)
{
	int i, off = SDBM_PBLKSIZ;
	short *ino = (short *) pag;
	
	for (i = 1; i < n; i += 2) {
		if (siz == off - ino[i] && memcmp(key, pag + ino[i], siz) == 0)
			return i;
		
		off = ino[i + 1];
	}
	
	return 0;
}

int fitpair(char *pag, int need)
{
	int n, off, avail;
	short *ino = (short *) pag;
	
	off = ((n = ino[0]) > 0) ? ino[n] : SDBM_PBLKSIZ;
	avail = off - (n + 1) * sizeof(short);
	need += 2 * sizeof(short);
	
	return need <= avail;
}

void putpair(char *pag, DATUM key, DATUM val)
{
	int n, off;
	short *ino = (short *) pag;
	
	off = ((n = ino[0]) > 0) ? ino[n] : SDBM_PBLKSIZ;
	
	off -= key.dsize;
	memcpy(pag + off, key.dptr, key.dsize);
	ino[n + 1] = off;
	
	off -= val.dsize;
	memcpy(pag + off, val.dptr, val.dsize);
	ino[n + 2] = off;
	
	ino[0] += 2;
}

int duppair(char *pag, DATUM key)
{
	short *ino = (short *) pag;
	return ino[0] > 0 && seepair(pag, ino[0], key.dptr, key.dsize) > 0;
}

DATUM getpair(char *pag, DATUM key)
{
	int i, n;
	DATUM val;
	short *ino = (short *) pag;
	
	if ((n = ino[0]) == 0)
		return nullDATUM;
	
	if ((i = seepair(pag, n, key.dptr, key.dsize)) == 0)
		return nullDATUM;
	
	val.dptr = pag + ino[i + 1];
	val.dsize = ino[i] - ino[i + 1];
	return val;
}

DATUM getnkey(char *pag, int num)
{
	DATUM key;
	int off;
	short *ino = (short *) pag;
	
	num = num * 2 - 1;
	
	if (ino[0] == 0 || num > ino[0])
		return nullDATUM;
	
	off = (num > 1) ? ino[num - 1] : SDBM_PBLKSIZ;
	
	key.dptr = pag + ino[num];
	key.dsize = off - ino[num];
	
	return key;
}

int delpair(char *pag, DATUM key)
{
	int n, i;
	short *ino = (short *) pag;
	
	if ((n = ino[0]) == 0)
		return 0;
	
	if ((i = seepair(pag, n, key.dptr, key.dsize)) == 0)
		return 0;
	
	if (i < n - 1) {
		int m;
		char *dst = pag + (i == 1 ? SDBM_PBLKSIZ : ino[i - 1]);
		char *src = pag + ino[i + 1];
		int   zoo = dst - src;
		
		m = ino[i + 1] - ino[n];
		
		dst -= m;
		src -= m;
		memmove(dst, src, m);
		
		while (i < n - 1) {
			ino[i] = ino[i + 2] + zoo;
			i++;
		}
	}
	
	ino[0] -= 2;
	return 1;
}

void splpage(char *pag, char *new, long sbit)
{
	DATUM key;
	DATUM val;
	
	int n, off = SDBM_PBLKSIZ;
	char cur[SDBM_PBLKSIZ];
	short *ino = (short *) cur;
	
	memcpy(cur, pag, SDBM_PBLKSIZ);
	bzero(pag, SDBM_PBLKSIZ);
	bzero(new, SDBM_PBLKSIZ);
	
	n = ino[0];
	
	for (ino++; n > 0; ino += 2) {
		key.dptr = cur + ino[0]; 
		key.dsize = off - ino[0];
		val.dptr = cur + ino[1];
		val.dsize = ino[0] - ino[1];
		
		putpair((exhash(key) & sbit) ? new : pag, key, val);
		
		off = ino[1];
		n -= 2;
	}
}

int chkpage(char *pag)
{
	int n, off;
	short *ino = (short *) pag;
	
	if ((n = ino[0]) < 0 || (unsigned int) n > SDBM_PBLKSIZ / sizeof(short))
		return 0;
	
	if (n > 0) {
		off = SDBM_PBLKSIZ;
		
		for (ino++; n > 0; ino += 2) {
			if (ino[0] > off || ino[1] > off || ino[1] > ino[0])
				return 0;
			
			off = ino[1];
			n -= 2;
		}
	}
	
	return 1;
}

int getdbit(SDBM *db, long dbit)
{
	long c;
	long dirb;
	
	c = dbit / BYTESIZ;
	dirb = c / SDBM_DBLKSIZ;
	
	if (dirb != db->dirbno) {
		if (lseek(db->dirf, OFF_DIR(dirb), SEEK_SET) < 0 ||
		    read(db->dirf, db->dirbuf, SDBM_DBLKSIZ) < 0)
			return 0;
		
		db->dirbno = dirb;
	}
	
	return db->dirbuf[c % SDBM_DBLKSIZ] & (1 << dbit % BYTESIZ);
}

int setdbit(SDBM *db, long dbit)
{
	long c;
	long dirb;
	
	c = dbit / BYTESIZ;
	dirb = c / SDBM_DBLKSIZ;
	
	if (dirb != db->dirbno) {
		if (lseek(db->dirf, OFF_DIR(dirb), SEEK_SET) < 0 ||
		    read(db->dirf, db->dirbuf, SDBM_DBLKSIZ) < 0)
			return 0;
		
		db->dirbno = dirb;
	}
	
	db->dirbuf[c % SDBM_DBLKSIZ] |= (1 << dbit % BYTESIZ);
	
	if (dbit >= db->maxbno)
		db->maxbno += SDBM_DBLKSIZ * BYTESIZ;
	
	if (lseek(db->dirf, OFF_DIR(dirb), SEEK_SET) < 0 ||
	    write(db->dirf, db->dirbuf, SDBM_DBLKSIZ) < 0)
		return 0;
	
	return 1;
}

int makroom(SDBM *db, long hash, int need)
{
	long newp;
	char twin[SDBM_PBLKSIZ];
	char *pag = db->pagbuf;
	char *new = twin;
	int smax = SDBM_SPLTMAX;
	
	do {
		splpage(pag, new, db->hmask + 1);
		
		newp = (hash & db->hmask) | (db->hmask + 1);
		
		if (hash & (db->hmask + 1)) {
			if (lseek(db->pagf, OFF_PAG(db->pagbno), SEEK_SET) < 0 ||
			    write(db->pagf, db->pagbuf, SDBM_PBLKSIZ) < 0)
				return 0;
			
			db->pagbno = newp;
			memcpy(pag, new, SDBM_PBLKSIZ);
		}
		
		else if (lseek(db->pagf, OFF_PAG(newp), SEEK_SET) < 0 ||
		         write(db->pagf, new, SDBM_PBLKSIZ) < 0)
			return 0;
		
		if (!setdbit(db, db->curbit))
			return 0;
		
		if (fitpair(pag, need))
			return 1;
		
		db->curbit = 2 * db->curbit + ((hash & (db->hmask + 1)) ? 2 : 1);
		db->hmask |= db->hmask + 1;
		
		if (lseek(db->pagf, OFF_PAG(db->pagbno), SEEK_SET) < 0 ||
		    write(db->pagf, db->pagbuf, SDBM_PBLKSIZ) < 0)
			return 0;
	} while (--smax);
	
	return 0;
}

int getpage(SDBM *db, long hash)
{
	int hbit;
	long dbit;
	long pagb;
	
	dbit = 0;
	hbit = 0;
	
	while (dbit < db->maxbno && getdbit(db, dbit))
		dbit = 2 * dbit + ((hash & (1 << hbit++)) ? 2 : 1);
	
	db->curbit = dbit;
	db->hmask = masks[hbit];
	
	pagb = hash & db->hmask;
	
	if (pagb != db->pagbno) {
		if (lseek(db->pagf, OFF_PAG(pagb), SEEK_SET) < 0 ||
		    read(db->pagf, db->pagbuf, SDBM_PBLKSIZ) < 0)
			return 0;
		
		if (!chkpage(db->pagbuf))
			return 0;
		
		db->pagbno = pagb;
	}
	
	return 1;
}

DATUM getnext(SDBM *db)
{
	DATUM key;
	
	for (;;) {
		db->keyptr++;
		
		key = getnkey(db->pagbuf, db->keyptr);
		
		if (key.dptr != NULL)
			return key;
		
		db->keyptr = 0;
		
		if (db->pagbno != db->blkptr++)
			if (lseek(db->pagf, OFF_PAG(db->blkptr), SEEK_SET) < 0)
				break;
		
		db->pagbno = db->blkptr;
		
		if (read(db->pagf, db->pagbuf, SDBM_PBLKSIZ) <= 0)
			break;
		
		if (!chkpage(db->pagbuf))
			break;
	}
	
	return ioerr(db), nullDATUM;
}

int sdbm_lock(int fd, int ro)
{
	int rc;
	
	while ((rc = flock(fd, ro ? LOCK_SH : LOCK_EX)) == -1 && errno == EINTR)
		continue;
	
	return rc;
}

int sdbm_unlock(int fd)
{
	return flock(fd, LOCK_UN);
}

#define HASHC  n = *str++ + 65599 * n

long sdbm_hash(char *str, int len)
{
	int loop;
	unsigned long n = 0;
	
	if (len > 0) {
		loop = (len + 8 - 1) >> 3;
		
		switch(len & (8 - 1)) {
		case 0:
			do {
				HASHC;
				case 7: HASHC;
				case 6: HASHC;
				case 5: HASHC;
				case 4: HASHC;
				case 3: HASHC;
				case 2: HASHC;
				case 1: HASHC;
			} while (--loop);
		}
	}
	
	return n;
}

#undef HASHC
