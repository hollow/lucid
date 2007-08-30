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

#include <fcntl.h>

#include "open.h"

int open_append(const char *filename)
{
	return open(filename, O_WRONLY|O_NONBLOCK|O_APPEND|O_CREAT, 0666);
}

int open_excl(const char *filename)
{
	return open(filename, O_WRONLY|O_NONBLOCK|O_CREAT|O_EXCL, 0666);
}

int open_read(const char *filename)
{
	return open(filename, O_RDONLY|O_NONBLOCK);
}

int open_rw(const char *filename)
{
	return open(filename, O_RDWR|O_NONBLOCK|O_CREAT, 0666);
}

int open_trunc(const char *filename)
{
	return open(filename, O_WRONLY|O_NONBLOCK|O_CREAT|O_TRUNC, 0666);
}

int open_write(const char *filename)
{
	return open(filename, O_WRONLY|O_NONBLOCK|O_CREAT, 0666);
}
