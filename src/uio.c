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
#include <dirent.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "char.h"
#include "cext.h"
#include "printf.h"
#include "str.h"
#include "strtok.h"
#include "uio.h"

int uio_open(const char *filename, const char *flags, mode_t mode)
{
	int f = O_NONBLOCK;

	if (!flags || !flags[0])
		return errno = EINVAL, -1;

	switch (flags[0]) {
	case 'a':
		if (flags[1] == '+')
			f |= O_RDWR;
		else
			f |= O_WRONLY;
		f |= O_APPEND|O_CREAT;
		break;

	case 'r':
		if (flags[1] == '+')
			f |= O_RDWR;
		else
			f |= O_RDONLY;
		break;

	case 'w':
		if (flags[1] == '+')
			f |= O_RDWR;
		else
			f |= O_WRONLY;
		f |= O_TRUNC|O_CREAT;
		break;

	default:
		return errno = EINVAL, -1;
	}

	return open(filename, f, mode ? mode : 0666);
}

int uio_read(int fd, char **str, int len)
{
	int buflen;
	char *buf = malloc(len + 1);

	if ((buflen = read(fd, buf, len)) == -1) {
		free(buf);
		return -1;
	}

	*str = buf;
	return buflen;
}

int uio_read_netstring(int fd, char **str)
{
	int ret = 0, len = 0, oldlen = 0;
	char c;

	while ((ret = read(fd, &c, 1)) == 1) {
		if (char_isdigit(c)) {
			len = (len * 10) + (c - '0');

			if (len < oldlen)
				return errno = ERANGE, -1;

			oldlen = len;
		}

		else if (c == ':')
			break;

		else
			return errno = EINVAL, -1;
	}

	if (ret < 0)
		return -1;

	if (len == 0)
		return 0;

	char *buf = NULL;
	ret = uio_read(fd, &buf, len);

	if (read(fd, &c, 1) == 1) {
		if (c == ',') {
			*str = buf;
			return ret;
		}

		else {
			if (buf) free(buf);
			return errno = EINVAL, -1;
		}
	}

	else {
		if (buf) free(buf);
		return -1;
	}
}

int uio_write_netstring(int fd, char *str)
{
	size_t len = str_len(str);
	char *netstr = NULL;

	asprintf(&netstr, "%u:%s,", len, str);
	return write(fd, netstr, str_len(netstr));
}

int uio_read_eof(int fd, char **str)
{
	static const size_t CHUNKSIZE = 4096;
	int chunks = 1, len = 0;
	char *buf = malloc(chunks * CHUNKSIZE + 1);

	for (;;) {
		int bytes_read = read(fd, buf+len, CHUNKSIZE);

		if (bytes_read == -1) {
			free(buf);
			return -1;
		}

		len += bytes_read;
		buf[len] = '\0';

		if (bytes_read == 0)
			break;

		if (bytes_read == CHUNKSIZE) {
			chunks++;
			buf = realloc(buf, chunks * CHUNKSIZE + 1);
		}
	}

	*str = buf;
	return len;
}

int uio_read_eol(int fd, char **line)
{
	static const size_t CHUNKSIZE = 4096;
	int chunks = 1, len = 0;
	char *buf = malloc(chunks * CHUNKSIZE + 1);
	char c;

	while (1) {
		switch(read(fd, &c, 1)) {
		case -1:
			free(buf);
			return -1;

		case 0:
			goto out;

		default:
			if (c == '\n' || c == '\r')
				goto out;

			if (len >= chunks * CHUNKSIZE) {
				chunks++;
				buf = realloc(buf, chunks * CHUNKSIZE + 1);
			}

			buf[len++] = c;
			break;
		}
	}

out:
	*line = buf;
	return len;
}

void uio_close(int fd)
{
	close(fd);
}

int uio_isdir(const char *path)
{
	struct stat sb;
	return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

int uio_isfile(const char *path)
{
	struct stat sb;
	return stat(path, &sb) == 0 && S_ISREG(sb.st_mode);
}

int uio_islink(const char *path)
{
	struct stat sb;
	return stat(path, &sb) == 0 && S_ISLNK(sb.st_mode);
}

int uio_ispath(const char *path)
{
	struct stat sb;
	return stat(path, &sb) == 0;
}

int uio_ismount(const char *path)
{
	struct stat sb_path, sb_parent;
	int rc = 1;

	if (str_isempty(path))
		return 0;

	if (str_equal(path, "/"))
		return 1;

	char *parent = str_path_dirname(path);

	if (lstat(path, &sb_path) == -1 ||
			!S_ISDIR(sb_path.st_mode) ||
			lstat(parent, &sb_parent) == -1 ||
			sb_path.st_dev == sb_parent.st_dev)
		rc = 0;

	free(parent);
	return rc;
}

int uio_mkdir(const char *path, mode_t mode)
{
	int ok = 1;
	struct stat sb;

	if (str_isempty(path) || str_path_isdot(path))
		return errno = EINVAL, -1;

	strtok_t _st, *st = &_st, *p;

	int curdir = uio_open(".", "r", 0);

	if (curdir == -1)
		return -1;

	if (!strtok_init_str(st, path, "/", 0))
		return -1;

	strtok_for_each(st, p) {
		if (mkdir(p->token, 0755) == -1) {
			if (errno != EEXIST || stat(p->token, &sb) == -1) {
				ok = 0;
				break;
			}

			if (!S_ISDIR(sb.st_mode)) {
				errno = ENOTDIR;
				ok = 0;
				break;
			}
		}

		if (chdir(p->token) == -1) {
			ok = 0;
			break;
		}
	}

	if (ok && chmod(".", mode) == -1)
		ok = 0;

	fchdir(curdir);
	close(curdir);

	strtok_free(st);
	return ok ? 0 : -1;
}

int uio_mkdirname(const char *path, mode_t mode)
{
	if (str_isempty(path))
		return errno = EINVAL, -1;

	char *dname = str_path_dirname(path);
	int rc      = uio_mkdir(dname, mode);

	free(dname);

	return rc;
}

char *uio_readlink(const char *path)
{
	static const size_t CHUNKSIZE = 128;
	int chunks = 1, len = 0;
	char *buf = malloc(chunks * CHUNKSIZE + 1);

	while (1) {
		len = readlink(path, buf, chunks * CHUNKSIZE);

		if (len == -1) {
			free(buf);
			return NULL;
		}

		if (len >= chunks * CHUNKSIZE) {
			chunks++;
			buf = realloc(buf, chunks * CHUNKSIZE + 1);
		}

		else
			break;
	}

	buf[len] = '\0';

	return buf;
}

int uio_unlink(const char *path)
{
	struct stat sb;

	DIR *dp;
	struct dirent *d;

	int status = 0;
	char *p, *new_path;

	if (lstat(path, &sb) == -1) {
		if (errno == ENOENT)
			return 0;
		else
			return -1;
	}

	if (S_ISDIR(sb.st_mode)) {
		if (!(dp = opendir(path)))
			return -1;

		while ((d = readdir(dp))) {
			p = d->d_name;

			if (p && p[0] == '.' && (!p[1] || (p[1] == '.' && !p[2])))
				continue;

			_lucid_asprintf(&new_path, "%s/%s", path, d->d_name);

			if (uio_unlink(new_path) == -1)
				status = -1;

			free(new_path);
		}

		if (closedir(dp) == -1)
			return -1;

		if (rmdir(path) == -1)
			return -1;

		return status;
	}

	if (unlink(path) == -1)
		return -1;

	return 0;
}

static sigjmp_buf __uio_copy_sigjmp_env;

static
void __uio_copy_sigbus_handler(int sig)
{
	siglongjmp(__uio_copy_sigjmp_env, 1);
}

static
void __uio_copy_sparse_memcpy(void *dst, const void *src, int len)
{
	/* although sparse memcpy is not as efficient as pure read/write
	 * we decrease instructions by about 50-80% using long instead of
	 * char for most of the copy operation */
	long *dsti = dst;
	const long *srci = src;

	int leni = len / sizeof(long);
	int rest = len - sizeof(long) * leni;

	int i;

	for (i = 0; i < leni; srci++, dsti++, i++) {
		if (*srci != 0)
			*dsti = *srci;
	}

	char *dstc = (void *) dsti;
	const char *srcc = (const void *) srci;

	for (i = 0; i < rest; srcc++, dstc++, i++) {
		if (*srcc != 0)
			*dstc = *srcc;
	}
}

int uio_copy(int srcfd, int dstfd)
{
	static const size_t CHUNKSIZE = 4096 * 1024;
	int errno_orig;
	int rc = -1, bufsize = 0;
	void *srcbuf = MAP_FAILED, *dstbuf = MAP_FAILED;

	/* install SIGBUS handler for mmap */
	void (*oldhandler)(int) = signal(SIGBUS, __uio_copy_sigbus_handler);

	/* get file length */
	struct stat sb;

	if (fstat(srcfd, &sb) == -1)
		goto out;

	/* create sparse file */
	if (ftruncate(dstfd, sb.st_size) == -1)
		goto out;

	if (sb.st_size < 1) {
		rc = 0;
		goto out;
	}

	/* save environment for non-local jump */
	if (sigsetjmp(__uio_copy_sigjmp_env, 1) != 0)
		goto out;

	int offset = 0;

	while (offset < sb.st_size) {
		bufsize = sb.st_size - offset;
		bufsize = bufsize > CHUNKSIZE ? CHUNKSIZE : bufsize;

		/* map source file */
		srcbuf = mmap(0, bufsize, PROT_READ, MAP_SHARED, srcfd, offset);

		if (srcbuf == MAP_FAILED)
			goto out;

		/* map destination file */
		dstbuf = mmap(0, bufsize, PROT_WRITE, MAP_SHARED, dstfd, offset);

		if (dstbuf == MAP_FAILED)
			goto out;

		offset += bufsize;

		/* advise to sequential order (more aggressive read ahead) */
		madvise(srcbuf, bufsize, MADV_SEQUENTIAL);
		madvise(dstbuf, bufsize, MADV_SEQUENTIAL);

		/* copy memory area with sparse support */
		__uio_copy_sparse_memcpy(dstbuf, srcbuf, bufsize);

		munmap(srcbuf, bufsize);
		srcbuf = MAP_FAILED;

		munmap(dstbuf, bufsize);
		dstbuf = MAP_FAILED;
	}

	rc = 0;

out:
	if (srcbuf && srcbuf != MAP_FAILED)
		munmap(srcbuf, bufsize);

	if (dstbuf && dstbuf != MAP_FAILED)
		munmap(dstbuf, bufsize);

	errno_orig = errno;
	signal(SIGBUS, oldhandler);
	errno = errno_orig;
	return rc;
}
