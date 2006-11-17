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
 * @defgroup log Logging functions
 * @{
 */

#ifndef _LUCID_LOG_H

#include <stdbool.h>
#include <stdarg.h>
#include <libgen.h>

/*! @brief log system configuration data */
typedef struct {
	char *ident;
	bool  file;
	int   fd;
	bool  stderr;
	bool  time;
	bool  syslog;
	int   flags;
	int   facility;
	int   mask;
} log_options_t;

/*!
 * @brief open connection to logging system
 *
 * @param options log system configuration options
 */
void log_init(log_options_t *options);

void log_internal(int level, int strerr, const char *fmt, va_list ap);

/*! @brief simple trace helper */
#define LOG_TRACEME log_debug("[trace] %s (%s:%d)", \
                          __FUNCTION__, basename(__FILE__), __LINE__);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_emerg(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_alert(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_crit(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_error(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_warn(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_notice(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_info(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_debug(const char *fmt, ...);


/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_emerg_and_die(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_alert_and_die(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_crit_and_die(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_error_and_die(const char *fmt, ...);


/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_pemerg(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_palert(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_pcrit(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 */
int log_perror(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_pwarn(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_pnotice(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_pinfo(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 */
int log_pdebug(const char *fmt, ...);


/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_pemerg_and_die(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_palert_and_die(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_pcrit_and_die(const char *fmt, ...);

/*!
 * @brief write message to logging system
 *
 * @param fmt format string passed to printf(3)
 * @param ... variable number of arguments according to fmt
 */
void log_perror_and_die(const char *fmt, ...);

/*!
 * @brief close connection to logging system
 */
void log_close(void);

#define _LUCID_LOG_H

#endif

/*! @} log */
