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
 * @defgroup log Log system multiplexer
 *
 * The log system multiplexer allows the caller to send log messages to multiple
 * destinations; currently: syslog(3), file, stderr.
 *
 * An application can only open one connection to the multiplexer during
 * runtime. Another call to log_init() will replace the previous connection.
 *
 * log_init() opens a connection to the multiplexer for a program. The options
 * argument is a pointer to a log_options_t structure used for the multiplexer 
 * configuration.
 *
 * @see log_options_t
 * @see syslog(3)
 *
 * @{
 */

#ifndef _LUCID_LOG_H

#include <stdbool.h>
#include <stdarg.h>
#include <libgen.h>

/*!
 * @brief multiplexer configuration data
 *
 * - The string pointed to by ident is prepended to every message, and is
 *   typically set to the program name.
 * - The file argument enables (1) or disables (0) multiplexing to the
 *   file destination.
 * - The stderr argument enables (1) or disables (0) multiplexing to the
 *   stderr destination.
 * - The time argument enables (1) or disables (0) output of the time string in
 *   multiplexed messages; only used for the file and stderr destinations.
 * - The syslog argument enables (1) or disables (0) multiplexing to the
 *   syslog(3) destination.
 * - The flags argument specifies flags which control the operation of the
 *   multiplexer.
 * - The facility argument is used to specify what type of program is logging
 *   the message; only used for the syslog(3) destination.
 * - The mask argument is the lower level bound of messages being multiplexed.
 */
typedef struct {
	char *ident;    /*!< program identifier */
	bool  file;     /*!< file destination switch */
	int   fd;       /*!< file descriptor */
	bool  stderr;   /*!< stderr destination switch */
	bool  time;     /*!< time output switch */
	bool  syslog;   /*!< syslog destination switch */
	int   flags;    /*!< control flags */
	int   facility; /*!< program facility */
	int   mask;     /*!< lower level bound */
} log_options_t;

/*!
 * @brief initialize log message mutliplexer
 *
 * @param[in] options multiplexer configuration
 *
 * @see log_options_t
 */
void log_init(log_options_t *options);

void log_internal(int level, int strerr, const char *fmt, va_list ap);

/*! @brief simple trace helper */
#define LOG_TRACEME log_debug("[trace] %s (%s:%d)", \
                          __FUNCTION__, basename(__FILE__), __LINE__);

/*!
 * @brief send EMERG level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_emerg(const char *fmt, ...);

/*!
 * @brief send ALERT level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_alert(const char *fmt, ...);

/*!
 * @brief send CRIT level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_crit(const char *fmt, ...);

/*!
 * @brief send ERR level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_error(const char *fmt, ...);

/*!
 * @brief send WARNING level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_warn(const char *fmt, ...);

/*!
 * @brief send NOTICE level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_notice(const char *fmt, ...);

/*!
 * @brief send INFO level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_info(const char *fmt, ...);

/*!
 * @brief send DEBUG level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_debug(const char *fmt, ...);


/*!
 * @brief send EMERG level message to the multiplexer and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_emerg_and_die(const char *fmt, ...);

/*!
 * @brief send ALERT level message to the multiplexer and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_alert_and_die(const char *fmt, ...);

/*!
 * @brief send CRIT level message to the multiplexer and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_crit_and_die(const char *fmt, ...);

/*!
 * @brief send ERR level message to the multiplexer and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_error_and_die(const char *fmt, ...);


/*!
 * @brief send EMERG level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_pemerg(const char *fmt, ...);

/*!
 * @brief send ALERT level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_palert(const char *fmt, ...);

/*!
 * @brief send CRIT level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_pcrit(const char *fmt, ...);

/*!
 * @brief send ERR level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_FAILURE
 *
 * @see printf
 */
int log_perror(const char *fmt, ...);

/*!
 * @brief send WARNING level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_pwarn(const char *fmt, ...);

/*!
 * @brief send NOTICE level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_pnotice(const char *fmt, ...);

/*!
 * @brief send INFO level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_pinfo(const char *fmt, ...);

/*!
 * @brief send DEBUG level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_pdebug(const char *fmt, ...);


/*!
 * @brief send EMERG level message to the multiplexer, append strerror(errno) and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_pemerg_and_die(const char *fmt, ...);

/*!
 * @brief send ALERT level message to the multiplexer, append strerror(errno) and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_palert_and_die(const char *fmt, ...);

/*!
 * @brief send CRIT level message to the multiplexer, append strerror(errno) and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_pcrit_and_die(const char *fmt, ...);

/*!
 * @brief send ERR level message to the multiplexer, append strerror(errno) and exit(2)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @see printf
 * @see exit(2)
 */
void log_perror_and_die(const char *fmt, ...);

/*!
 * @brief close connection to logging system
 */
void log_close(void);

#define _LUCID_LOG_H

#endif

/*! @} log */
