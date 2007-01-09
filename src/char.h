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
 * @defgroup char Character classification and manipulation
 *
 * The char family of macros check whether ch, which must have the value of
 * an unsigned char, falls into a certain character class.
 *
 * char_isalnum() checks for an alphanumeric character; it is equivalent to
 * (char_isalpha(ch) || char_isdigit(ch)).
 *
 * char_isalpha() checks for an alphabetic character; it is equivalent to
 * (char_isupper(c) || char_islower(c)).
 *
 * char_isascii() checks whether ch is a 7-bit unsigned char value that fits
 * into the ASCII character set.
 *
 * char_isblank() checks for a blank character; that is, a space or a tab.
 *
 * char_iscntrl() checks for a control character.
 *
 * char_isdigit() checks for a digit (0 through 9).
 *
 * char_isgraph() checks for any printable character except space.
 *
 * char_islower() checks for a lower-case character.
 *
 * char_isprint() checks for any printable character including space.
 *
 * char_ispunct() checks for any printable character which is not a space or
 * an alphanumeric character.
 *
 * char_isspace() checks for white-space characters. These are: space,
 * form-feed ('\f'), newline ('\n'), carriage return ('\r'),
 * horizontal tab ('\t'), and vertical tab ('\v').
 *
 * char_isupper() checks for an uppercase letter.
 *
 * char_isxdigit() checks for a hexadecimal digits, i.e. one of 0 1 2 3 4 5 6 7
 * 8 9 a b c d e f A B C D E F.
 *
 * char_tolower() converts a character to lowercase if applicable.
 *
 * char_toupper() converts a character to uppercase if applicable.
 *
 * @{
 */

#ifndef _LUCID_CHAR_H
#define _LUCID_CHAR_H

/*! @brief check for an ASCII character */
#define char_isascii(ch) ((unsigned int)(ch) < 128u)

/*! @brief check for a blank character (space, horizontal tab) */
#define char_isblank(ch) (ch == ' ' || ch == '\t')

/*! @brief check for an ASCII control character */
#define char_iscntrl(ch) ((unsigned int)(ch) < 32u || ch == 127)

/*! @brief check for a digit character (0-9) */
#define char_isdigit(ch) ((unsigned int)(ch - '0') < 10u)

/*! @brief check for graphable characters (excluding space) */
#define char_isgraph(ch) ((unsigned int)(ch - '!') < 94u)

/*! @brief check for a lower-case character */
#define char_islower(ch) ((unsigned int)(ch - 'a') < 26u)

/*! @brief check for a printable character (including space) */
#define char_isprint(ch) ((unsigned int)(ch - ' ') < 95u)

/*! @brief check for a whitespace character (\\t, \\n, \\v, \\f, \\r) */
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

#endif

/*! @} char */
