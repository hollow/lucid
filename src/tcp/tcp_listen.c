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

#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "addr.h"
#include "mem.h"
#include "tcp.h"

int tcp_listen(const char *ip, int port, int backlog)
{
	int fd;
	struct sockaddr_in inaddr;
	
	if (port < 1)
		return errno = EINVAL, -1;
	
	mem_set(&inaddr, 0, sizeof(inaddr));
	inaddr.sin_family = AF_INET;
	inaddr.sin_port   = addr_htos(port);
	
	if (addr_from_str(ip, &inaddr.sin_addr.s_addr, 0) == 0)
		return errno = EINVAL, -1;
	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;
	
	if (bind(fd, (struct sockaddr *) &inaddr, sizeof(struct sockaddr_in)) == -1) {
		close(fd);
		return -1;
	}
	
	if (listen(fd, backlog) == -1) {
		close(fd);
		return -1;
	}
	
	return fd;
}
