## Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

noinst_HEADERS = doxygen/examples.h \
                 doxygen/license.h \
                 doxygen/main.h

pkginclude_HEADERS = addr.h \
                     chroot.h \
                     bitmap.h \
                     exec.h \
                     flist.h \
                     http.h \
                     list.h \
                     log.h \
                     mem.h \
                     misc.h \
                     open.h \
                     printf.h \
                     scanf.h \
                     str.h \
                     stralloc.h \
                     strtok.h \
                     tcp.h \
                     whirlpool.h

ADDR_C = addr/addr_from_str.c \
         addr/addr_hton.c \
         addr/addr_htos.c \
         addr/addr_ntoh.c \
         addr/addr_stoh.c \
         addr/addr_to_str.c

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

HTTP_C = http/http_header_add.c \
         http/http_header_del.c \
         http/http_header_free.c \
         http/http_header_init.c \
         http/http_internal.h \
         http/http_request_fromstr.c \
         http/http_request_tostr.c \
         http/http_response_fromstr.c \
         http/http_response_tostr.c \
         http/http_url_cmp.c \
         http/http_url_fromstr.c \
         http/http_url_tostr.c

LOG_C = log/log_close.c \
        log/log_init.c \
        log/log_internal.c

MEM_C = mem/mem_alloc.c \
        mem/mem_ccpy.c \
        mem/mem_chr.c \
        mem/mem_cmp.c \
        mem/mem_cpy.c \
        mem/mem_dup.c \
        mem/mem_freeall.c \
        mem/mem_free.c \
        mem/mem_idx.c \
        mem/mem_internal.h \
        mem/mem_realloc.c \
        mem/mem_set.c

MISC_C = misc/copy_file.c \
         misc/isdir.c \
         misc/isfile.c \
         misc/islink.c \
         misc/ismount.c \
         misc/ispath.c \
         misc/mkdirnamep.c \
         misc/mkdirp.c \
         misc/readsymlink.c \
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
        str/str_chr.c \
        str/str_cmp.c \
        str/str_cmpn.c \
        str/str_cpy.c \
        str/str_cpyn.c \
        str/str_dup.c \
        str/str_equal.c \
        str/str_len.c \
        str/str_path_basename.c \
        str/str_path_concat.c \
        str/str_path_dirname.c \
        str/str_path_isabs.c \
        str/str_path_isdot.c \
        str/str_rchr.c \
        str/str_read.c \
        str/str_readfile.c \
        str/str_readline.c \
        str/str_str.c \
        str/str_tolower.c \
        str/str_toumax.c \
        str/str_toupper.c

STRALLOC_C = stralloc/stralloc_catb.c \
             stralloc/stralloc_cat.c \
             stralloc/stralloc_catf.c \
             stralloc/stralloc_catm.c \
             stralloc/stralloc_cats.c \
             stralloc/stralloc_cmp.c \
             stralloc/stralloc_copyb.c \
             stralloc/stralloc_copy.c \
             stralloc/stralloc_copys.c \
             stralloc/stralloc_finalize.c \
             stralloc/stralloc_free.c \
             stralloc/stralloc_init.c \
             stralloc/stralloc_ready.c \
             stralloc/stralloc_readyplus.c \
             stralloc/stralloc_zero.c

STRTOK_C = strtok/strtok_append.c \
           strtok/strtok_count.c \
           strtok/strtok_delete.c \
           strtok/strtok_free.c \
           strtok/strtok_init_argv.c \
           strtok/strtok_init_str.c \
           strtok/strtok_next.c \
           strtok/strtok_prev.c \
           strtok/strtok_toargv.c \
           strtok/strtok_tostr.c

TCP_C = tcp/tcp_connect.c \
        tcp/tcp_listen.c

WHIRLPOOL_C = whirlpool/whirlpool_add.c \
              whirlpool/whirlpool_digest.c \
              whirlpool/whirlpool_finalize.c \
              whirlpool/whirlpool_init.c \
              whirlpool/whirlpool_tables.h \
              whirlpool/whirlpool_transform.c


lib_LTLIBRARIES = libucid.la

libucid_la_SOURCES = $(ADDR_C) \
                     $(BITMAP_C) \
                     $(CHROOT_C) \
                     $(EXEC_C) \
                     $(FLIST_C) \
                     $(HTTP_C) \
                     $(LOG_C) \
                     $(MEM_C) \
                     $(MISC_C) \
                     $(OPEN_C) \
                     $(PRINTF_C) \
                     $(SCANF_C) \
                     $(STR_C) \
                     $(STRALLOC_C) \
                     $(STRTOK_C) \
                     $(TCP_C) \
                     $(WHIRLPOOL_C)

libucid_la_LDFLAGS = -version-info 0:0:0
