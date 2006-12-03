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
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "chroot.h"
#include "log.h"

static
int chroot_secure_chdir_t(void)
{
	int fd;
	char path[PATH_MAX];
	char *tempd, tempt[] = "/tmp/chroottest-XXXXXX";
	
	if ((fd = open(".", O_RDONLY|O_DIRECTORY)) == -1)
		return log_perror("[%s] open(.)", __FUNCTION__);
	
	struct stat sbcwd;
	
	if (stat(".", &sbcwd) == -1)
		return log_perror("[%s] stat(.)", __FUNCTION__);
	
	if (!(tempd = mkdtemp(tempt)))
		return log_perror("[%s] mkdtemp(%s)", __FUNCTION__, tempt);
	
	snprintf(path, PATH_MAX, "%s/etc", tempd);
	
	if (mkdir(path, 0755) == -1)
		return log_perror("[%s] mkdir(%s)", __FUNCTION__, path);
	
	struct stat sb;
	
	if (stat(path, &sb) == -1)
		return log_perror("[%s] stat(%s)", __FUNCTION__, path);
	
	snprintf(path, PATH_MAX, "%s/link", tempd);
	
	if (symlink("/etc", path) == -1)
		return log_perror("[%s] symlink(/etc, %s)", __FUNCTION__, path);
	
	if (chroot_secure_chdir(tempd, "/link") == -1)
		return log_perror("[%s] chroot_secure_chdir(%s, /link)", __FUNCTION__, tempd);
	
	struct stat sb2;
	
	if (stat(".", &sb2) == -1)
		return log_perror("[%s] stat(.)", __FUNCTION__);
	
	if (sb.st_dev != sb2.st_dev || sb.st_ino != sb2.st_ino)
		return log_error("[%s/01] E[%u,%u] R[%u,%u]",
		                 sb.st_dev, sb.st_ino, sb2.st_dev, sb2.st_ino);
	
	if (fchdir(fd) == -1)
		return log_perror("[%s] fchdir(%d)", __FUNCTION__, fd);
	
	if (stat(".", &sb2) == -1)
		return log_perror("[%s] stat(.)", __FUNCTION__);
	
	if (sbcwd.st_dev != sb2.st_dev || sbcwd.st_ino != sb2.st_ino)
		return log_error("[%s/02] E[%u,%u] R[%u,%u]",
		                 sb.st_dev, sb.st_ino, sb2.st_dev, sb2.st_ino);
	
	return 0;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;
	
	log_options_t log_options = {
		.ident  = "chroot",
		.stderr = true,
		.time   = false,
	};
	
	log_init(&log_options);
	
	rc += chroot_secure_chdir_t();
	
	log_close();
	
	return rc;
}
