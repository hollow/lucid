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

#include <unistd.h>
#include <errno.h>

#include "chroot.h"
#include "uio.h"

/* go to <dir> in <root> as root
** going into the chroot before doing chdir(dir) prevents symlink attacks
** and hence is safer */
int chroot_secure_chdir(const char *root, const char *dir)
{
	int orig_root, new_root;

	if ((orig_root = uio_open("/", "r", 0)) == -1)
		return -1;

	if (chdir(root) == -1)
		return -1;

	if ((new_root = uio_open(".", "r", 0)) == -1)
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
	dirfd = uio_open(".", "r", 0);

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
