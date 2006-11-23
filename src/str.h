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
 * @defgroup str String classification and conversion
 *
 * The char family of functions check whether ch, which must have the value of
 * an unsigned char, falls into a certain character class.
 *
 * The str_check family of functions extend the classification of single
 * characters to strings. The str_check() function checks the string pointed to
 * by str for a set of allowed character classes. As soon as a character is
 * found that is not allowed checking stops and 0 is returned.
 *
 * The str_cmp() function compares the string pointed to by str1 to the string
 * pointed to by str2. It returns an integer less than, equal to, or greater
 * than zero if str1 is found, respectively, to be less than, to match, or be
 * greater than str2.
 *
 * The strcpy() function copies the string pointed to by src (including the
 * terminating `\\0' character) to the array pointed to by dst. The strings may
 * not overlap, and the destination string dst must be large enough to receive
 * the copy. The strncpy() function is similar, except that not more than n
 * bytes of src are copied. Thus, if there is no null byte among the first n
 * bytes of src, the result will not be null-terminated.
 *
 * The str_dup() function returns a pointer to a new string which is a duplicate
 * of the string str. The str_dupn() function is similar, but only copies at
 * most n characters. If s is longer than n, only n characters are copied, and a
 * terminating null byte is added.
 *
 * The str_index() returns a pointer to the first occurence of the character c
 * in the string pointed to by str.
 *
 * The str_len() function calculates the length of the string str, not including
 * the terminating `\\0' character.
 *
 * The str_path_concat() function concatenates the directory name pointed to by
 * dirname and file name pointed to by basename and checks that the latter does
 * not contain any dot entries.
 *
 * The str_path_isabs() and str_path_isdot() functions check if the file path
 * pointed to by str is absolute or contains dots, respectively.
 *
 * The str_toupper() and str_tolower() functions map lower-case to upper case
 * and vice-versa, respectively.
 *
 * The str_zero() function sets the first n bytes of the byte area starting at s
 * to zero (bytes containing '\\0').
 *
 * The str_toumax() function converts the string pointed to by str to an
 * unsigned long long int val using base as conversion base.
 *
 * @{
 */

#ifndef _LUCID_STR_H
#define _LUCID_STR_H

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


/*! @brief class for alpha-numerical characters */
#define CC_ALNUM  (1 <<  1)

/*! @brief class for upper- or lower-case characters */
#define CC_ALPHA  (1 <<  2)

/*! @brief class for ASCII characters */
#define CC_ASCII  (1 <<  3)

/*! @brief class for blank characters */
#define CC_BLANK  (1 <<  4)

/*! @brief class for ASCII control characters */
#define CC_CNTRL  (1 <<  5)

/*! @brief class for digit characters */
#define CC_DIGIT  (1 <<  6)

/*! @brief class for graphable characters */
#define CC_GRAPH  (1 <<  7)

/*! @brief class for lower-case characters */
#define CC_LOWER  (1 <<  8)

/*! @brief class for printable characters */
#define CC_PRINT  (1 <<  9)

/*! @brief class for punctuation characters */
#define CC_PUNCT  (1 << 10)

/*! @brief class for white space characters */
#define CC_SPACE  (1 << 11)

/*! @brief class for upper-case characters */
#define CC_UPPER  (1 << 12)

/*! @brief class for hexadecimal characters */
#define CC_XDIGIT (1 << 13)

/*!
 * @brief check string against classes of allowed characters
 *
 * @param[in] str     string to check
 * @param[in] allowed allowed classes of characters (multiple classes by ORing)
 *
 * @return 1 if all characters are valid, 0 otherwise
 */
int str_check(const char *str, int allowed);

/*! @brief check if string is empty */
#define str_isempty(str)  (!str || str_check(str, CC_SPACE))

/*! @brief check string for alpha-numerical characters */
#define str_isalnum(str)  str_check(str, CC_ALNUM)

/*! @brief check string for upper- or lower-case characters */
#define str_isalpha(str)  str_check(str, CC_ALPHA)

/*! @brief check string for ASCII characters */
#define str_isascii(str)  str_check(str, CC_ASCII)

/*! @brief check string for digit characters */
#define str_isdigit(str)  str_check(str, CC_DIGIT)

/*! @brief check string for graphable characters */
#define str_isgraph(str)  str_check(str, CC_GRAPH)

/*! @brief check string for lower-case characters */
#define str_islower(str)  str_check(str, CC_LOWER)

/*! @brief check string for printable characters */
#define str_isprint(str)  str_check(str, CC_PRINT)

/*! @brief check string for upper-case characters */
#define str_isupper(str)  str_check(str, CC_UPPER)

/*! @brief check string for hexadecimal characters */
#define str_isxdigit(str) str_check(str, CC_XDIGIT)


/*!
 * @brief compare two strings
 *
 * @param[in] str1 first string
 * @param[in] str2 second string
 *
 * @return An integer greater than, equal to, or less than 0, if the string
 *         pointed to by str1 is greater than, equal to, or less than the string
 *         pointed to by str2, respectively.
 */
int str_cmp(const char *str1, const char *str2);

/*!
 * @brief copy a string
 *
 * @param[out] dst destination string
 * @param[in]  src source string
 *
 * @return Number of bytes that have been copied.
 */
int str_cpy(char *dst, const char *src);

/*!
 * @brief copy a string
 *
 * @param[out] dst destination string
 * @param[in]  src source string
 * @param[in]  n   copy first n bytes
 *
 * @return Number of bytes that have been copied.
 */
int str_cpyn(void *dst, const void *src, int n);

/*!
 * @brief duplicate a string
 *
 * @param[in] str source string
 *
 * @return A pointer to the duplicated string, or NULL if insufficient memory
 *         was available.
 */
char *str_dup(const char *str);

/*!
 * @brief duplicate a string
 *
 * @param[in] str source string
 * @param[in] n   string length
 *
 * @return A pointer to the duplicated string, or NULL if insufficient memory
 *         was available.
 */
char *str_dupn(const char *str, int n);

/*!
 * @brief scan string for character
 *
 * @param[in] str string to scan
 * @param[in] c   character to look for
 * @param[in] n   scan first n bytes
 *
 * @return A pointer to the matched character or NULL if the character is not
 *         found.
 */
char *str_index(const char *str, int c, int n);

/*!
 * @brief calculate the length of a string
 *
 * @param[in] str source string
 *
 * @return number of characters in str
 */
int str_len(const char *str);

/*!
 * @brief write zero-valued bytes
 *
 * @param[out] str destination string
 * @param[in]  n   write first n bytes
 */
void str_zero(void *str, int n);


/*!
 * @brief concatenate dirname and basename
 *
 * @param[in] dirname  directory part
 * @param[in] basename basename part
 *
 * @return A pointer to the newly allocated string or NULL if insufficent memory
 *         was available.
 */
char *str_path_concat(const char *dirname, const char *basename);

/*!
 * @brief check if path is absolute and contains no dot entries or ungraphable characters
 *
 * @param[in] str path to check
 *
 * @return 1 if str is an absolute pathname, 0 otherwise
 *
 * @note this function does not check if the path exists
 */
int str_path_isabs(const char *str);

/*!
 * @brief check if given path contains . or .. entries
 *
 * @param[in] str path to check
 *
 * @return 1 if str has dot entries, 0 otherwise
 */
int str_path_isdot(const char *str);


/*!
 * @brief convert string to lower-case
 *
 * @param[out] str string to convert
 *
 * @return pointer to str
 */
char *str_tolower(char *str);

/*!
 * @brief convert string to upper-case
 *
 * @param[out] str string to convert
 *
 * @return pointer to str
 */
char *str_toupper(char *str);


/*!
 * @brief convert string to integer
 *
 * @param[in]  str  source string
 * @param[out] val  destination integer
 * @param[in]  base conversion base
 * @param[in]  n    convert first n bytes
 *
 * @return Number of bytes read from str
 */
int str_toumax(const char *str, unsigned long long int *val, int base, int n);

#endif

/*! @} str */
