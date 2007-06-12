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

#include <errno.h>

#include "asm/unistd.h"
#include "asm/page.h"

#include "sys.h"
#include "syscall.h"

/* getpagesize */
#ifdef __NR_getpagesize
_syscall0(int, getpagesize)
#else
int sys_getpagesize(void)
{
	return PAGE_SIZE;
}
#endif

/* mmap & friends */
_syscall6(void *, mmap, void *, start, size_t, length, int, prot, int, flags, int, fd, off_t, off)
_syscall4(void *, mremap, void *, old, size_t, old_size, size_t, new_size, int, flags)
_syscall2(int, munmap, void *, start, size_t, length)
