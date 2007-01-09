// Copyright (C) 2005      Felix von Leitner <felix-libowfat@fefe.de>
//               2006-2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#include <errno.h>

#include "stralloc.h"

int stralloc_readyplus(stralloc_t *sa, size_t len)
{
	if (sa->s) {
		if (sa->len + len < len)
			return errno = EINVAL, -1;
		
		return stralloc_ready(sa, sa->len + len);
	}
	
	return stralloc_ready(sa, len);
}
