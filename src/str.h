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

/*!
 * @defgroup str String classification and conversion
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
#define str_isempty(str)  (!str || str_check(str, CC_BLANK))

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
 * @brief compare two strings
 *
 * @param[in] str1 first string
 * @param[in] str2 second string
 * @param[in] n    compare first n bytes
 *
 * @return An integer greater than, equal to, or less than 0, if the string
 *         pointed to by str1 is greater than, equal to, or less than the string
 *         pointed to by str2, respectively.
 */
int str_cmpn(const char *str1, const char *str2, int n);

/*!
 * @brief compare two strings
 *
 * @param[in] str1 first string
 * @param[in] str2 second string
 *
 * @return 1 if both strings are equal, 0 otherwise.
 */
int str_equal(const char *str1, const char *str2);

/*!
 * @brief copy a string
 *
 * @param[out] dst destination string
 * @param[in]  src source string
 *
 * @return A pointer to dst.
 */
char *str_cpy(char *dst, const char *src);

/*!
 * @brief copy a string
 *
 * @param[out] dst destination string
 * @param[in]  src source string
 * @param[in]  n   copy at most n bytes
 *
 * @return A pointer to dst.
 */
char *str_cpyn(char *dst, const char *src, int n);

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
 * @brief scan string for character
 *
 * @param[in] str string to scan
 * @param[in] c   character to look for
 * @param[in] n   scan first n bytes
 *
 * @return A pointer to the matched character or NULL if the character is not
 *         found.
 */
char *str_chr(const char *str, int c, int n);

/*!
 * @brief scan string for character beginning at the end
 *
 * @param[in] str string to scan
 * @param[in] c   character to look for
 * @param[in] n   scan first n bytes
 *
 * @return A pointer to the matched character or NULL if the character is not
 *         found.
 */
char *str_rchr(const char *str, int c, int n);

/*!
 * @brief locate a substring
 *
 * @param[in] str    string to scan
 * @param[in] needle string to look for
 *
 * @return A pointer to the matched substring or NULL if the substring is not
 *         found.
 */
char *str_str(const char *str, const char *needle);

/*!
 * @brief calculate the length of a string
 *
 * @param[in] str source string
 *
 * @return number of characters in str
 */
int str_len(const char *str);


/*!
 * @brief parse directory component
 *
 * @param[in] path path to parse
 *
 * @return A pointer to the newly allocated string or NULL if insufficent memory
 *         was available.
 */
char *str_path_dirname(const char *path);

/*!
 * @brief parse basename component
 *
 * @param[in] path path to parse
 *
 * @return A pointer to the newly allocated string or NULL if insufficent memory
 *         was available.
 */
char *str_path_basename(const char *path);

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


/*! bytes read at a time */
#define CHUNKSIZE 4096

/*!
 * @brief read a line of input
 *
 * @param[in]  fd   file descriptor to read from
 * @param[out] line pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for line using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int str_readline(int fd, char **str);

/*!
 * @brief read until end of file
 *
 * @param[in]  fd   file descriptor to read from
 * @param[out] file pointer to a string
 *
 * @return bytes on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for file using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int str_readfile(int fd, char **str);

/*!
 * @brief read exact number of bytes
 *
 * @param[in]  fd  file descriptor to read from
 * @param[out] str pointer to a string
 * @param[in]  len bytes to be read
 *
 * @return bytes read on success, -1 on error with errno set
 *
 * @note The caller should free obtained memory for str using free(3)
 *
 * @see malloc(3)
 * @see free(3)
 * @see read(2)
 */
int str_read(int fd, char **str, int len);

#endif

/*! @} str */
