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
#define _LUCID_LOG_H

#include <stdarg.h>

/* destinations */
#define LOGD_SYSLOG 0x01 /*!< Log to syslog */
#define LOGD_FILE   0x02 /*!< Log to a file */
#define LOGD_STDERR 0x04 /*!< Log to STDERR */

/* priorities */
#define LOGP_ALERT 0 /*!< action must be taken immediately */
#define LOGP_ERROR 1 /*!< error conditions */
#define LOGP_WARN  2 /*!< warning conditions */
#define LOGP_NOTE  3 /*!< normal but significant condition */
#define LOGP_INFO  4 /*!< informational */
#define LOGP_DEBUG 5 /*!< debug-level messages */
#define LOGP_TRACE 6 /*!< trace messages */

/* options */
#define LOGO_PID   0x01 /*!< log the pid with each message */
#define LOGO_TIME  0x02 /*!< log the time with each message */
#define LOGO_PRIO  0x04 /*!< log the priority with each message */
#define LOGO_IDENT 0x08 /*!< log the ident string with each message */

/*! @brief simple trace helper */
#define LOG_TRACEME log_traceme(__FILE__, __FUNCTION__, __LINE__);

/*!
 * @brief multiplexer configuration data
 *
 * - The string pointed to by log_ident is prepended to every message, and is
 *   typically set to the program name.
 * - The log_dest argument specifies the log destination
 * - The log_facility argument is used to specify what type of program is logging
 *   the message; only used for the syslog(3) destination.
 * - The log_opts argument specifies flags which control the operation of the
 *   multiplexer.
 * - The log_mask argument is the lower level bound of messages being multiplexed.
 */
typedef struct {
	const char *log_ident; /*!< program identifier */
	int log_dest;          /*!< file destination switch */
	int log_fd;            /*!< file descriptor for LOGD_FILE target */
	int log_facility;      /*!< program facility */
	int log_opts;          /*!< control flags */
	int log_mask;          /*!< lower log level bound */
} log_options_t;

/*!
 * @brief initialize log message mutliplexer
 *
 * @param[in] options multiplexer configuration
 *
 * @see log_options_t
 */
void log_init(log_options_t *options);

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
 * @brief send TRACE level message to the multiplexer
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_trace(const char *fmt, ...);

/*!
 * @brief send TRACE level message to the multiplexer
 *
 * @param[in] file File name
 * @param[in] func Function name
 * @param[in] line Line number
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_traceme(const char *file, const char *func, int line);


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
 * @brief send TRACE level message to the multiplexer and append strerror(errno)
 *
 * @param[in] fmt format string passed to printf
 * @param[in] ... variable number of arguments according to fmt
 *
 * @return EXIT_SUCCESS
 *
 * @see printf
 */
int log_ptrace(const char *fmt, ...);


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

#endif

/*! @} log */
