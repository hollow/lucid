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

#include <string.h>

#include "http/http.h"

http_method_t http_str_to_method(const char *str)
{
	int i;
	
	for (i = 0; _http_method_map[i].name; i++)
		if (strcmp(_http_method_map[i].name, str) == 0)
			return _http_method_map[i].method;
	
	return HTTP_METHOD_INVALID;
}
