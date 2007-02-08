// Copyright (C) 2007 Benedikt Böhm <hollow@gentoo.org>
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
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "misc.h"
#include "open.h"

static sigjmp_buf __copy_file_sigjmp_env;

static
void __copy_file_sigbus_handler(int sig)
{
	siglongjmp(__copy_file_sigjmp_env, 1);
}

static
void __copy_file_sparse_memcpy(void *dst, const void *src, int len)
{
	/* although sparse memcpy is not as efficient as pure read/write
	 * we decrease instructions by about 60% using int instead of
	 * char for most of the copy operation */
	int *dsti = dst;
	const int *srci = src;

	int leni = len / sizeof(int);
	int rest = len - sizeof(int) * leni;

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

#define CHUNKSIZE (16*1024*1024) /* 16M */

int copy_fileat(int srcdirfd, const char *src, int dstdirfd, const char *dst)
{
	int errno_orig;
	int rc = -1, bufsize = 0;
	int srcfd = -1, dstfd = -1;
	void *srcbuf = MAP_FAILED, *dstbuf = MAP_FAILED;

	/* install SIGBUS handler for mmap */
	void (*oldhandler)(int) = signal(SIGBUS, __copy_file_sigbus_handler);

	/* open source and destination files */
	srcfd = openat(srcdirfd, src,
			O_RDONLY|O_NOCTTY|O_NONBLOCK|O_NOFOLLOW|O_LARGEFILE);

	if (srcfd == -1)
		goto out;

	/* get file length */
	struct stat sb;

	if (fstat(srcfd, &sb) == -1)
		goto out;

	/* open destination file */
	dstfd = openat(dstdirfd, dst, O_RDWR|O_CREAT|O_EXCL|O_NOCTTY, 0200);

	if (dstfd == -1)
		goto out;

	if (sb.st_size < 1) {
		rc = 0;
		goto out;
	}

	/* create sparse file */
	if (ftruncate(dstfd, sb.st_size) == -1)
		goto out_unlink;

	/* save environment for non-local jump */
	if (sigsetjmp(__copy_file_sigjmp_env, 1) != 0)
		goto out_unlink;

	int offset = 0;

	while (offset < sb.st_size) {
		bufsize = sb.st_size - offset;
		bufsize = bufsize > CHUNKSIZE ? CHUNKSIZE : bufsize;

		/* map source file */
		srcbuf = mmap(0, bufsize, PROT_READ, MAP_SHARED, srcfd, offset);

		if (srcbuf == MAP_FAILED)
			goto out_unlink;

		/* map destination file */
		dstbuf = mmap(0, bufsize, PROT_WRITE, MAP_SHARED, dstfd, offset);

		if (dstbuf == MAP_FAILED)
			goto out_unlink;

		offset += bufsize;

		/* advise to sequential order (more aggressive read ahead) */
		madvise(srcbuf, bufsize, MADV_SEQUENTIAL);
		madvise(dstbuf, bufsize, MADV_SEQUENTIAL);

		/* copy memory area with sparse support */
		__copy_file_sparse_memcpy(dstbuf, srcbuf, bufsize);

		munmap(srcbuf, bufsize);
		srcbuf = MAP_FAILED;

		munmap(dstbuf, bufsize);
		dstbuf = MAP_FAILED;
	}

	rc = 0;
	goto out_unmap;

out_unlink:
	errno_orig = errno;
	unlink(dst);
	errno = errno_orig;

out_unmap:
	errno_orig = errno;

	if (srcbuf && srcbuf != MAP_FAILED)
		munmap(srcbuf, bufsize);

	if (dstbuf && dstbuf != MAP_FAILED)
		munmap(dstbuf, bufsize);

	errno = errno_orig;

out:
	errno_orig = errno;

	signal(SIGBUS, oldhandler);

	close(srcfd);
	close(dstfd);

	errno = errno_orig;
	return rc;
}
