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

ADDR_H = addr/addr.h
ADDR_C = addr/addr_from_str.c \
         addr/addr_to_str.c

ARGV_H = argv/argv.h
ARGV_C = argv/argv_from_str.c \
         argv/argv_to_str.c

CHROOT_H = chroot/chroot.h
CHROOT_C = chroot/chroot_fd.c \
           chroot/chroot_secure_chdir.c

EXEC_H = exec/exec.h
EXEC_C = exec/exec_fork.c \
         exec/exec_fork_background.c \
         exec/exec_replace.c

FLIST_H = flist/flist.h
FLIST_C = flist/flist32_getkey.c \
          flist/flist32_getval.c \
          flist/flist32_mask2val.c \
          flist/flist32_parse.c \
          flist/flist32_tostr.c \
          flist/flist32_val2mask.c \
          flist/flist64_getkey.c \
          flist/flist64_getval.c \
          flist/flist64_mask2val.c \
          flist/flist64_parse.c \
          flist/flist64_tostr.c \
          flist/flist64_val2mask.c

FMT_H = fmt/fmt.h
FMT_C = fmt/fmt_8long.c \
        fmt/fmt_8longlong.c \
        fmt/fmt_double.c \
        fmt/fmt_fill.c \
        fmt/fmt_long.c \
        fmt/fmt_longlong.c \
        fmt/fmt_minus.c \
        fmt/fmt_pad.c \
        fmt/fmt_plusminus.c \
        fmt/fmt_str.c \
        fmt/fmt_strn.c \
        fmt/fmt_ulong.c \
        fmt/fmt_ulonglong.c \
        fmt/fmt_xlong.c \
        fmt/fmt_xlonglong.c

HTTP_H = http/http.h
HTTP_C = http/http_get_request.c \
         http/http_get_response.c \
         http/http_internal.c \
         http/http_method_to_str.c \
         http/http_read_callback.c \
         http/http_send_request.c \
         http/http_send_response.c \
         http/http_status_to_str.c \
         http/http_str_to_method.c \
         http/http_str_to_status.c \
         http/http_write_callback.c

IO_H = io/io.h
IO_C = io/io_read_eof.c \
       io/io_read_eol.c \
       io/io_read_len.c

MMAP_H = mmap/mmap.h
MMAP_C = mmap/mmap_private.c \
         mmap/mmap_read.c \
         mmap/mmap_shared.c

OPEN_H = open/open.h
OPEN_C = open/open_append.c \
         open/open_excl.c \
         open/open_read.c \
         open/open_rw.c \
         open/open_trunc.c \
         open/open_write.c

PRINTF_H = printf/printf.h
PRINTF_C = printf/asprintf.c \
           printf/dprintf.c \
           printf/printf.c \
           printf/snprintf.c \
           printf/vasprintf.c \
           printf/vdprintf.c \
           printf/vprintf.c \
           printf/vsnprintf.c

SDBM_H = sdbm/sdbm.h
SDBM_C = sdbm/sdbm_internal.c \
         sdbm/sdbm_close.c \
         sdbm/sdbm_delete.c \
         sdbm/sdbm_fetch.c \
         sdbm/sdbm_firstkey.c \
         sdbm/sdbm_nextkey.c \
         sdbm/sdbm_open.c \
         sdbm/sdbm_store.c

STRALLOC_H = stralloc/stralloc.h
STRALLOC_C = stralloc/stralloc_catb.c \
             stralloc/stralloc_cat.c \
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

SYS_H = sys/sys.h \
        sys/syscall.h
SYS_C = sys/sys_clone.c

TST_H = tst/tst.h
TST_C = tst/tst_cleanup.c \
        tst/tst_delete.c \
        tst/tst_init.c \
        tst/tst_insert.c \
        tst/tst_search.c

noinst_HEADERS = $(ADDR_H) \
                 $(ARGV_H) \
                 $(CHROOT_H) \
                 $(EXEC_H) \
                 $(FLIST_H) \
                 $(FMT_H) \
                 $(HTTP_H) \
                 $(IO_H) \
                 $(MMAP_H) \
                 $(OPEN_H) \
                 $(PRINTF_H) \
                 $(SDBM_H) \
                 $(STRALLOC_H) \
                 $(SYS_H) \
                 $(TST_H) \
                 lucid.h

noinst_LIBRARIES = libucid.a

libucid_a_SOURCES = $(ADDR_C) \
                    $(ARGV_C) \
                    $(CHROOT_C) \
                    $(EXEC_C) \
                    $(FLIST_C) \
                    $(FMT_C) \
                    $(HTTP_C) \
                    $(IO_C) \
                    $(MMAP_C) \
                    $(OPEN_C) \
                    $(PRINTF_C) \
                    $(SDBM_C) \
                    $(STRALLOC_C) \
                    $(SYS_C) \
                    $(TST_C)
