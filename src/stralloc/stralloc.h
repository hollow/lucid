// Copyright 2005 Felix von Leitner <felix-libowfat@fefe.de>
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

#ifndef _LUCID_STRALLOC_H
#define _LUCID_STRALLOC_H

#include <stdlib.h>

typedef struct {
	char *s;
	size_t len;
	size_t a;
} STRALLOC;

void stralloc_init     (STRALLOC *sa);
void stralloc_zero     (STRALLOC *sa);
int  stralloc_ready    (STRALLOC *sa, size_t len);
int  stralloc_readyplus(STRALLOC *sa, size_t len);
void stralloc_free     (STRALLOC *sa);

int  stralloc_copyb    (STRALLOC *dst, char *src, size_t len);
int  stralloc_copys    (STRALLOC *dst, char *src);
int  stralloc_copy     (STRALLOC *dst, STRALLOC *src);

int  stralloc_catb     (STRALLOC *dst, char *src, size_t len);
int  stralloc_cats     (STRALLOC *dst, char *src);
int  stralloc_cat      (STRALLOC *dst, STRALLOC *src);

int  stralloc_cmp      (STRALLOC *a, STRALLOC *b);

#endif
