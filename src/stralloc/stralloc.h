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

#include <sys/types.h>

typedef struct {
	char *s;
	size_t len;
	size_t a;
} stralloc_t;

void stralloc_init     (stralloc_t *sa);
void stralloc_zero     (stralloc_t *sa);
int  stralloc_ready    (stralloc_t *sa, size_t len);
int  stralloc_readyplus(stralloc_t *sa, size_t len);
void stralloc_free     (stralloc_t *sa);

int  stralloc_copyb    (stralloc_t *dst, const char *src, size_t len);
int  stralloc_copys    (stralloc_t *dst, const char *src);
int  stralloc_copy     (stralloc_t *dst, const stralloc_t *src);

int  stralloc_catb     (stralloc_t *dst, const char *src, size_t len);
int  stralloc_catf     (stralloc_t *dst, const char *fmt, ...);
int  stralloc_catm     (stralloc_t *dst, ...);
int  stralloc_cats     (stralloc_t *dst, const char *src);
int  stralloc_cat      (stralloc_t *dst, const stralloc_t *src);

int  stralloc_cmp      (const stralloc_t *a, const stralloc_t *b);

#endif
