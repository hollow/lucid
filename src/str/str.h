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

int str_isempty(const char *str);

int str_isalnum(const char *str);
int str_isalpha(const char *str);
int str_isascii(const char *str);
int str_isblank(const char *str);
int str_isdigit(const char *str);
int str_isgraph(const char *str);
int str_islower(const char *str);
int str_isprint(const char *str);
int str_ispunct(const char *str);
int str_isspace(const char *str);
int str_isupper(const char *str);
int str_isxdigit(const char *str);

#endif
