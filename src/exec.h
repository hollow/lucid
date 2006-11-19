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
 * @defgroup exec Command execution wrappers
 *
 * The exec family of functions provide convenient wrappers around fork(2),
 * execve(2), waitpid(2) and pipe(2).
 *
 * These functions combine one or more of the above system calls in one
 * function, thus allowing fast and simple process creation in applications.
 *
 * @{
 */

#ifndef _LUCID_EXEC_H
#define _LUCID_EXEC_H

/*!
 * @brief maximum number of arguments that will be converted for execvp(2)
 */
#define EXEC_MAX_ARGV 64

/*!
 * @brief fork, execvp and wait
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return status obtained by wait(2) or -1 with errno set
 *
 * @see printf
 * @see execvp(2)
 */
int exec_fork(const char *fmt, ...);

/*!
 * @brief fork, execvp and ignore child
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return 0 on success or -1 with errno set
 *
 * @see printf
 * @see execvp(2)
 *
 * @note this function closes file descriptors 0-100 before execvp
 */
int exec_fork_background(const char *fmt, ...);

/*!
 * @brief pipe, fork, execvp and wait
 *
 * @param[out] out empty pointer to store combined stdout/stderr
 * @param[in]  fmt format string passed to printf
 * @param[in]  ... variable number of arguments according to fmt
 *
 * @return status obtained by wait(2) or -1 with errno set
 *
 * @note The caller should free obtained memory for out using free(3)
 *
 * @see printf
 * @see malloc(3)
 * @see free(3)
 * @see execvp(2)
 */
int exec_fork_pipe(char **out, const char *fmt, ...);

/*!
 * @brief plain execvp
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return only returns on error with errno set
 *
 * @see printf
 * @see execvp(2)
 */
int exec_replace(const char *fmt, ...);

#endif

/*! @} exec */
