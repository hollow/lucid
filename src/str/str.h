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

#ifndef _LUCID_STR_H
#define _LUCID_STR_H

#define char_isascii(ch) ((unsigned int)(ch      ) < 128u)
#define char_isblank(ch) (ch == ' ' || ch == '\t')
#define char_iscntrl(ch) ((unsigned int)(ch      ) < 32u || ch == 127)
#define char_isdigit(ch) ((unsigned int)(ch - '0') < 10u)
#define char_isgraph(ch) ((unsigned int)(ch - '!') < 94u)
#define char_islower(ch) ((unsigned int)(ch - 'a') < 26u)
#define char_isprint(ch) ((unsigned int)(ch - ' ') < 95u)
#define char_isspace(ch) ((unsigned int)(ch - '\t') < 5u || ch == ' ')
#define char_isupper(ch) ((unsigned int)(ch - 'A') < 26u)
#define char_isxdigit(ch) (char_isdigit(ch) || \
                          (unsigned int)(ch - 'a') < 6u || \
                          (unsigned int)(ch - 'A') < 6u)

#define char_isalpha(ch) (char_islower(ch) || char_isupper(ch))
#define char_isalnum(ch) (char_isalpha(ch) || char_isdigit(ch))
#define char_ispunct(ch) (char_isprint(ch) && \
                         !char_isalnum(ch) && \
                         !char_isspace(ch))

#define char_tolower(ch) do { if (char_isupper(ch)) ch += 32; } while(0)
#define char_toupper(ch) do { if (char_islower(ch)) ch -= 32; } while(0)

#define CC_ALNUM  (1 <<  1)
#define CC_ALPHA  (1 <<  2)
#define CC_ASCII  (1 <<  3)
#define CC_BLANK  (1 <<  4)
#define CC_CNTRL  (1 <<  5)
#define CC_DIGIT  (1 <<  6)
#define CC_GRAPH  (1 <<  7)
#define CC_LOWER  (1 <<  8)
#define CC_PRINT  (1 <<  9)
#define CC_PUNCT  (1 << 10)
#define CC_SPACE  (1 << 11)
#define CC_UPPER  (1 << 12)
#define CC_XDIGIT (1 << 13)

int str_check(const char *str, int allowed);

#define str_isempty(str)  str_check(str, CC_SPACE)
#define str_isalnum(str)  str_check(str, CC_ALNUM)
#define str_isalpha(str)  str_check(str, CC_ALPHA)
#define str_isascii(str)  str_check(str, CC_ASCII)
#define str_isdigit(str)  str_check(str, CC_DIGIT)
#define str_isgraph(str)  str_check(str, CC_GRAPH)
#define str_islower(str)  str_check(str, CC_LOWER)
#define str_isprint(str)  str_check(str, CC_PRINT)
#define str_isupper(str)  str_check(str, CC_UPPER)
#define str_isxdigit(str) str_check(str, CC_XDIGIT)

int str_path_isdot(const char *str);
int str_path_isabs(const char *str);

char *str_tolower(char *str);
char *str_toupper(char *str);

#endif
