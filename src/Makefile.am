## Copyright 2005 The vserver-utils Developers
## See AUTHORS for details
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by  *
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the
## Free Software Foundation, Inc.,
## 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

EXTRA_DIST = Doxyfile

noinst_HEADERS = doxygen.h

pkginclude_HEADERS = addr.h \
                     argv.h \
                     chroot.h \
                     bitmap.h \
                     exec.h \
                     flist.h \
                     io.h \
                     list.h \
                     log.h \
                     misc.h \
                     open.h \
                     printf.h \
                     scanf.h \
                     str.h \
                     stralloc.h \
                     tcp.h \
                     whirlpool.h

ADDR_C = addr/addr_from_str.c \
         addr/addr_to_str.c

ARGV_C = argv/argv_from_str.c \
         argv/argv_to_str.c

BITMAP_C = bitmap/i2v32.c \
           bitmap/i2v64.c \
           bitmap/v2i32.c \
           bitmap/v2i64.c

CHROOT_C = chroot/chroot_fd.c \
           chroot/chroot_mkdirp.c \
           chroot/chroot_secure_chdir.c

EXEC_C = exec/exec_fork.c \
         exec/exec_fork_background.c \
         exec/exec_fork_pipe.c \
         exec/exec_replace.c

FLIST_C = flist/flist32_from_str.c \
          flist/flist32_getkey.c \
          flist/flist32_getval.c \
          flist/flist32_to_str.c \
          flist/flist64_from_str.c \
          flist/flist64_getkey.c \
          flist/flist64_getval.c \
          flist/flist64_to_str.c

IO_C = io/io_read_eof.c \
       io/io_read_eol.c \
       io/io_read_len.c

LOG_C = log/log_close.c \
        log/log_init.c \
        log/log_internal.c

MISC_C = misc/isdir.c \
         misc/isfile.c \
         misc/islink.c \
         misc/mkdirnamep.c \
         misc/mkdirp.c \
         misc/runlink.c

OPEN_C = open/open_append.c \
         open/open_excl.c \
         open/open_read.c \
         open/open_rw.c \
         open/open_trunc.c \
         open/open_write.c

PRINTF_C = printf/asprintf.c \
           printf/dprintf.c \
           printf/printf.c \
           printf/snprintf.c \
           printf/vasprintf.c \
           printf/vdprintf.c \
           printf/vprintf.c \
           printf/vsnprintf.c

SCANF_C = scanf/sscanf.c \
          scanf/vsscanf.c

STR_C = str/str_check.c \
        str/str_cmp.c \
        str/str_cpy.c \
        str/str_cpyn.c \
        str/str_dup.c \
        str/str_dupn.c \
        str/str_index.c \
        str/str_len.c \
        str/str_path_concat.c \
        str/str_path_isabs.c \
        str/str_path_isdot.c \
        str/str_tolower.c \
        str/str_toumax.c \
        str/str_toupper.c \
        str/str_zero.c

STRALLOC_C = stralloc/stralloc_catb.c \
             stralloc/stralloc_cat.c \
             stralloc/stralloc_catf.c \
             stralloc/stralloc_catm.c \
             stralloc/stralloc_cats.c \
             stralloc/stralloc_cmp.c \
             stralloc/stralloc_copyb.c \
             stralloc/stralloc_copy.c \
             stralloc/stralloc_copys.c \
             stralloc/stralloc_free.c \
             stralloc/stralloc_init.c \
             stralloc/stralloc_ready.c \
             stralloc/stralloc_readyplus.c \
             stralloc/stralloc_zero.c

TCP_C = tcp/tcp_connect.c \
        tcp/tcp_listen.c

WHIRLPOOL_C = whirlpool/whirlpool_add.c \
              whirlpool/whirlpool_digest.c \
              whirlpool/whirlpool_finalize.c \
              whirlpool/whirlpool_init.c \
              whirlpool/whirlpool_internal.h \
              whirlpool/whirlpool_tables.h \
              whirlpool/whirlpool_transform.c


lib_LTLIBRARIES = libucid.la

libucid_la_SOURCES = $(ADDR_C) \
                     $(ARGV_C) \
                     $(BITMAP_C) \
                     $(CHROOT_C) \
                     $(EXEC_C) \
                     $(FLIST_C) \
                     $(IO_C) \
                     $(LOG_C) \
                     $(MISC_C) \
                     $(OPEN_C) \
                     $(PRINTF_C) \
                     $(SCANF_C) \
                     $(STR_C) \
                     $(STRALLOC_C) \
                     $(TCP_C) \
                     $(WHIRLPOOL_C)

libucid_la_LDFLAGS = -version-info 0:0:0
