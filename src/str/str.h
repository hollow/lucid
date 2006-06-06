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

/*!
 * @defgroup str String checks
 * @{
 */

#ifndef _LUCID_STR_H
#define _LUCID_STR_H

/*! @brief check for an ASCII character */
#define char_isascii(ch) ((unsigned int)(ch      ) < 128u)

/*! @brief check for a blank character (space, horizontal tab) */
#define char_isblank(ch) (ch == ' ' || ch == '\t')

/*! @brief check for an ASCII control character */
#define char_iscntrl(ch) ((unsigned int)(ch      ) < 32u || ch == 127)

/*! @brief check for a digit character (0-9) */
#define char_isdigit(ch) ((unsigned int)(ch - '0') < 10u)

/*! @brief check for graphable characters (excluding space) */
#define char_isgraph(ch) ((unsigned int)(ch - '!') < 94u)

/*! @brief check for a lower-case character */
#define char_islower(ch) ((unsigned int)(ch - 'a') < 26u)

/*! @brief check for a printable character (including space) */
#define char_isprint(ch) ((unsigned int)(ch - ' ') < 95u)

/*! @brief check for a whitespace character (\t, \n, \v, \f, \r) */
#define char_isspace(ch) ((unsigned int)(ch - '\t') < 5u || ch == ' ')

/*! @brief check for an upper-case character */
#define char_isupper(ch) ((unsigned int)(ch - 'A') < 26u)

/*! @brief check for a hexadecimal character */
#define char_isxdigit(ch) (char_isdigit(ch) || \
                          (unsigned int)(ch - 'a') < 6u || \
                          (unsigned int)(ch - 'A') < 6u)


/*! @brief check for an upper- or lower-case character */
#define char_isalpha(ch) (char_islower(ch) || char_isupper(ch))

/*! @brief check for an upper-, lower-case or digit character */
#define char_isalnum(ch) (char_isalpha(ch) || char_isdigit(ch))

/*! @brief check for a punctuation character */
#define char_ispunct(ch) (char_isprint(ch) && \
                         !char_isalnum(ch) && \
                         !char_isspace(ch))


/*! @brief convert character to lower-case */
#define char_tolower(ch) do { if (char_isupper(ch)) ch += 32; } while(0)

/*! @brief convert character to upper-case */
#define char_toupper(ch) do { if (char_islower(ch)) ch -= 32; } while(0)


/*! @brief check string for alpha-numerical characters */
#define CC_ALNUM  (1 <<  1)

/*! @brief check string for upper- or lower-case characters */
#define CC_ALPHA  (1 <<  2)

/*! @brief check string for ASCII characters */
#define CC_ASCII  (1 <<  3)

/*! @brief check string for blank characters */
#define CC_BLANK  (1 <<  4)

/*! @brief check string for ASCII control characters */
#define CC_CNTRL  (1 <<  5)

/*! @brief check string for digit characters */
#define CC_DIGIT  (1 <<  6)

/*! @brief check string for graphable characters */
#define CC_GRAPH  (1 <<  7)

/*! @brief check string for lower-case characters */
#define CC_LOWER  (1 <<  8)

/*! @brief check string for printable characters */
#define CC_PRINT  (1 <<  9)

/*! @brief check string for punctuation characters */
#define CC_PUNCT  (1 << 10)

/*! @brief check string for white space characters */
#define CC_SPACE  (1 << 11)

/*! @brief check string for upper-case characters */
#define CC_UPPER  (1 << 12)

/*! @brief check string for hexadecimal characters */
#define CC_XDIGIT (1 << 13)

/*!
 * @brief check string against set of allowed characters
 *
 * @param str     string to check
 * @param allowed allowed set of characters (multiple sets by ORing)
 *
 * @return 1 if all characters are valid, 0 otherwise
 */
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

/*!
 * @brief check if given path contains . or .. entries
 *
 * @param str path to check
 *
 * @return 1 if str has dot entries, 0 otherwise
 */
int str_path_isdot(const char *str);

/*!
 * @brief check if path is absolute and contains no dot entries or ungraphable characters
 *
 * @param str path to check
 *
 * @return 1 if str is an absolute pathname, 0 otherwise
 *
 * @note this function does not check if the path exists
 */
int str_path_isabs(const char *str);

/*!
 * @brief convert string to lower-case
 *
 * @param str string to convert
 *
 * @return pointer to str
 */
char *str_tolower(char *str);

/*!
 * @brief convert string to upper-case
 *
 * @param str string to convert
 *
 * @return pointer to str
 */
char *str_toupper(char *str);

#endif

/*! @} str */
