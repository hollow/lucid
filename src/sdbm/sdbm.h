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

#ifndef _LUCID_SDBM_H
#define _LUCID_SDBM_H

#define SDBM_DBLKSIZ  4096
#define SDBM_PBLKSIZ  1024
#define SDBM_PAIRMAX  1008    /* arbitrary on PBLKSIZ-N */
#define SDBM_SPLTMAX  10      /* maximum allowed splits for a single insertion */
#define SDBM_DIRFEXT  ".dir"
#define SDBM_PAGFEXT  ".pag"

typedef struct {
	int dirf;                   /* directory file descriptor */
	int pagf;                   /* page file descriptor */
	int flags;                  /* status/error flags, see below */
	long maxbno;                /* size of dirfile in bits */
	long curbit;                /* current bit number */
	long hmask;                 /* current hash mask */
	long blkptr;                /* current block for nextkey */
	int keyptr;                 /* current key for nextkey */
	long blkno;                 /* current page to read/write */
	long pagbno;                /* current page in pagbuf */
	char pagbuf[SDBM_PBLKSIZ];  /* page file block buffer */
	long dirbno;                /* current block in dirbuf */
	char dirbuf[SDBM_DBLKSIZ];  /* directory file block buffer */
} SDBM;

/* SDBM.flags */
#define SDBM_RDONLY 0x1   /* data base open read-only */
#define SDBM_IOERR  0x2   /* data base I/O error */

/* utility macros */
#define sdbm_rdonly(db)   ((db)->flags & SDBM_RDONLY)
#define sdbm_error(db)    ((db)->flags & SDBM_IOERR)
#define sdbm_clearerr(db) ((db)->flags &= ~SDBM_IOERR)

typedef struct {
	char *dptr;
	int dsize;
} DATUM;

/* flags to sdbm_store */
#define SDBM_INSERT   0
#define SDBM_REPLACE  1

/* public interface */
void  sdbm_close   (SDBM *db);
int   sdbm_delete  (SDBM *db, DATUM key);
DATUM sdbm_fetch   (SDBM *db, DATUM key);
DATUM sdbm_firstkey(SDBM *db);
DATUM sdbm_nextkey (SDBM *db);
SDBM *sdbm_open    (char *file, int flags, int mode);
int   sdbm_store   (SDBM *db, DATUM key, DATUM value, int flags);

/* private functions */
#ifdef SDBM_INTERNAL
#define BYTESIZ 8

#define bad(x)        ((x).dptr == NULL || (x).dsize <= 0)
#define exhash(item)  sdbm_hash((item).dptr, (item).dsize)
#define ioerr(db)     ((db)->flags |= SDBM_IOERR)

#define OFF_PAG(off)  (long) (off) * SDBM_PBLKSIZ
#define OFF_DIR(off)  (long) (off) * SDBM_DBLKSIZ

extern DATUM nullDATUM;

int   seepair(char *pag, int n, char *key, int siz);
int   fitpair(char *pag, int need);
void  putpair(char *pag, DATUM key, DATUM val);
DATUM getpair(char *pag, DATUM key);
DATUM getnkey(char *pag, int num);
int   delpair(char *pag, DATUM key);
void  splpage(char *pag, char *new, long sbit);
int   chkpage(char *pag);

int   getdbit(SDBM *db, long dbit);
int   setdbit(SDBM *db, long dbit);
int   makroom(SDBM *db, long hash, int need);
int   getpage(SDBM *db, long hash);
DATUM getnext(SDBM *db);

int   sdbm_lock  (int fd, int ro);
int   sdbm_unlock(int fd);
long  sdbm_hash  (char *str, int len);
#endif

#endif
