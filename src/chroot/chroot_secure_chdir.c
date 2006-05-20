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

/* go to <dir> in <root> as root
** going into the chroot before doing chdir(dir) prevents symlink attacks
** and hence is safer */
int chroot_secure_chdir(char *root, char *dir)
{
	int orig_root, new_root;
	
	if ((orig_root = open("/",  O_RDONLY)) == -1)
		return -1;
	
	if (chdir(root) == -1)
		return -1;
	
	if ((new_root = open(".",  O_RDONLY)) == -1)
		return -1;
	
	int dirfd;
	int errno_orig;
	
	/* check cwdfd */
	if (chroot_fd(new_root) == -1)
		return -1;
	
	/* now go to dir in the chroot */
	if (chdir(dir) == -1)
		goto err;
	
	/* save a file descriptor of the target dir */
	dirfd = open(".", O_RDONLY);
	
	if (dirfd == -1)
		goto err;
	
	/* break out of the chroot */
	chroot_fd(orig_root);
	
	/* now go to the saved target dir (but outside the chroot) */
	if (fchdir(dirfd) == -1)
		goto err2;
	
	close(dirfd);
	return 0;
	
err2:
	errno_orig = errno;
	close(dirfd);
	errno = errno_orig;
err:
	errno_orig = errno;
	chroot_fd(orig_root);
	errno = errno_orig;
	return -1;
}
