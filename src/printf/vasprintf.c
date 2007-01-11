// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
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

#include <stdlib.h>

#include "mem.h"
#include "printf.h"

int _lucid_vasprintf(char **ptr, const char *fmt, va_list ap)
{
	va_list ap2;
	int len;
	char *buf;
	
	/* don't consume the original ap, we'll need it again */
	va_copy(ap2, ap);
	
	/* get required size */
	len = _lucid_vsnprintf(0, 0, fmt, ap2);
	
	va_end(ap2);
	
	/* if size is 0, no buffer is allocated
	** just set *ptr to NULL and return size */
	if (len > 0) {
		if (!(buf = mem_alloc(len + 1)))
			return -1;
		
		_lucid_vsnprintf(buf, len, fmt, ap);
		
		*ptr = buf;
	}
	
	return len;
}
