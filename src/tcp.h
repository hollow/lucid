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
 * @defgroup tcp TCP socket wrappers
 *
 * The tcp family of functions provide wrappers around connect(2) and listen(2)
 * taking an IP address in the string pointed to by ip and the port number as
 * arguments.
 *
 * @{
 */

#ifndef _LUCID_TCP_H
#define _LUCID_TCP_H

/*!
 * @brief listen for incomming connections
 *
 * @param ip      IP to listen on
 * @param port    port to listen on
 * @param backlog queue backlog
 *
 * @return filedescriptor for the newly allocated socket, -1 on error with errno set
 */
int tcp_listen(const char *ip, int port, int backlog);

/*!
 * @brief connect to TCP socket
 *
 * @param ip      IP to connect to
 * @param port    port to connect to
 *
 * @return filedescriptor for the newly allocated connection, -1 on error with errno set
 */
int tcp_connect(const char *ip, int port);

#endif

/*! @} tcp */
