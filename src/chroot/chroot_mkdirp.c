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
#include <fcntl.h>

#include "chroot/chroot.h"
#include "misc/misc.h"

/* go to <dir> in <root> as root
** going into the chroot before doing chdir(dir) prevents symlink attacks
** and hence is safer */
int chroot_mkdirp(char *root, char *dir, mode_t mode)
{
	int orig_root, new_root;
	int errno_orig;
	
	if ((orig_root = open("/",  O_RDONLY)) == -1)
		return -1;
	
	if (chdir(root) == -1)
		return -1;
	
	if ((new_root = open(".",  O_RDONLY)) == -1)
		return -1;
	
	/* check cwdfd */
	if (chroot_fd(new_root) == -1)
		return -1;
	
	/* now create the dir in the chroot */
	if (mkdirp(dir, mode) == -1)
		goto err;
	
	/* break out of the chroot */
	chroot_fd(orig_root);
	
	return 0;
	
err:
	errno_orig = errno;
	chroot_fd(orig_root);
	errno = errno_orig;
	return -1;
}
