// Copyright 2006 Benedikt Böhm <hollow@gentoo.org>
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

#ifndef _LIBUCID_FLIST_H
#define _LIBUCID_FLIST_H

#include <stdint.h>

typedef struct {
	char *key;
	uint32_t val;
} flist32_t;

typedef struct {
	char *key;
	uint64_t val;
} flist64_t;

#define FLIST32_START(LIST) const flist32_t LIST[] = {
#define FLIST32_NODE(PREFIX, NAME) { #NAME, PREFIX ## _ ## NAME },
#define FLIST32_NODE1(PREFIX, NAME) { #NAME, (1 << PREFIX ## _ ## NAME) },
#define FLIST32_END { NULL, 0 } };

#define FLIST64_START(LIST) const flist64_t LIST[] = {
#define FLIST64_NODE(PREFIX, NAME) { #NAME, PREFIX ## _ ## NAME },
#define FLIST64_NODE1(PREFIX, NAME) { #NAME, (1 << PREFIX ## _ ## NAME) },
#define FLIST64_END { NULL, 0 } };

int flist32_getval(const flist32_t list[], char *key, uint32_t *val);
int flist64_getval(const flist64_t list[], char *key, uint64_t *val);
int flist32_getkey(const flist32_t list[], uint32_t val, char **key);
int flist64_getkey(const flist64_t list[], uint64_t val, char **key);
int flist32_mask2val(uint32_t mask);
int flist64_mask2val(uint64_t mask);
uint32_t flist32_val2mask(int val);
uint64_t flist64_val2mask(int val);
int flist32_parse(char *str, const flist32_t list[],
                  uint32_t *flags, uint32_t *mask,
                  char clmod, char delim);
int flist64_parse(char *str, const flist64_t list[],
                  uint64_t *flags, uint64_t *mask,
                  char clmod, char delim);
int flist32_tostr(const flist32_t list[], uint32_t val, char **str, char delim);
int flist64_tostr(const flist64_t list[], uint64_t val, char **str, char delim);

#endif
