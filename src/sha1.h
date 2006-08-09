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

#ifndef _LUCID_SHA1_H
#define _LUCID_SHA1_H

typedef struct {
	unsigned long state[5];
	unsigned long count[2];
	unsigned char buffer[64];
} sha1_t;

char *sha1_digest(const char *str);

void sha1_final(unsigned char digest[20], sha1_t* context);
void sha1_init(sha1_t* context);
void sha1_transform(unsigned long state[5], unsigned char buffer[64]);
void sha1_update(sha1_t* context, unsigned char* data, unsigned int len);

#endif
