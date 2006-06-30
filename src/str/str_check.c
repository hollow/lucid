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

#include "str/str.h"

int str_check(const char *str, int allowed)
{
	int i, n;
	
	if (!str)
		return 1;
	
	n = strlen(str);
	
	for (i = 0; i < n; i++) {
		if (allowed & CC_ALNUM  && char_isalnum (str[i])) continue;
		if (allowed & CC_ALPHA  && char_isalpha (str[i])) continue;
		if (allowed & CC_ASCII  && char_isascii (str[i])) continue;
		if (allowed & CC_BLANK  && char_isblank (str[i])) continue;
		if (allowed & CC_CNTRL  && char_iscntrl (str[i])) continue;
		if (allowed & CC_DIGIT  && char_isdigit (str[i])) continue;
		if (allowed & CC_GRAPH  && char_isgraph (str[i])) continue;
		if (allowed & CC_LOWER  && char_islower (str[i])) continue;
		if (allowed & CC_PRINT  && char_isprint (str[i])) continue;
		if (allowed & CC_PUNCT  && char_ispunct (str[i])) continue;
		if (allowed & CC_SPACE  && char_isspace (str[i])) continue;
		if (allowed & CC_UPPER  && char_isupper (str[i])) continue;
		if (allowed & CC_XDIGIT && char_isxdigit(str[i])) continue;
		
		return 0;
	}
	
	return 1;
}
