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
	char *d = dst;
	const char *s = src;
	int i;

	/* do not copy null bytes (sparse sections) to not dirty pages */
	for (i = 0; i < len; i++, d++, s++)
		if (*s != 0)
			*d = *s;
}

#define CHUNKSIZE (16*1024*1024) /* 16M */

int copy_file(const char *src, const char *dst)
{
	int rc = -1, bufsize = 0;
	void *srcbuf = 0, *dstbuf = 0;

	/* open source and destination files */
	int srcfd = open_read(src);

	if (srcfd == -1)
		return -1;

	int dstfd = open_trunc(dst);

	if (dstfd == -1) {
		close(srcfd);
		return -1;
	}

	/* install SIGBUS handler for mmap */
	void (*oldhandler)(int) = signal(SIGBUS, __copy_file_sigbus_handler);

	/* get file length */
	struct stat sb;

	if (fstat(srcfd, &sb) == -1)
		goto out;

	if (sb.st_size < 1) {
		rc = 0;
		goto out;
	}

	/* create sparse file */
	if (ftruncate(dstfd, sb.st_size) == -1)
		goto out;

	/* save environment for non-local jump */
	if (sigsetjmp(__copy_file_sigjmp_env, 1) != 0)
		goto out;

	int offset = 0;

	while (offset < sb.st_size) {
		bufsize = sb.st_size - offset;
		bufsize = bufsize > CHUNKSIZE ? CHUNKSIZE : bufsize;

		/* map source file */
		srcbuf = mmap(0, bufsize, PROT_READ, MAP_PRIVATE, srcfd, offset);

		if (srcbuf == MAP_FAILED)
			goto out;

		/* map destination file */
		dstbuf = mmap(0, bufsize, PROT_WRITE, MAP_PRIVATE, dstfd, offset);

		if (dstbuf == MAP_FAILED)
			goto out;

		offset += bufsize;

		/* advise to sequential order (more aggressive read ahead) */
		madvise(srcbuf, bufsize, MADV_SEQUENTIAL);
		madvise(dstbuf, bufsize, MADV_SEQUENTIAL);

		/* copy memory area with sparse support */
		__copy_file_sparse_memcpy(dstbuf, srcbuf, bufsize);

		munmap(srcbuf, bufsize);
		srcbuf = 0;

		munmap(dstbuf, bufsize);
		dstbuf = 0;
	}

out:
	if (srcbuf)
		munmap(srcbuf, bufsize);

	if (dstbuf)
		munmap(dstbuf, bufsize);

	signal(SIGBUS, oldhandler);

	close(srcfd);
	close(dstfd);
	return rc;
}
