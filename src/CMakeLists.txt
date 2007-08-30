include(CheckIncludeFile)

check_include_file(dirent.h     HAVE_DIRENT_H)
check_include_file(errno.h      HAVE_ERRNO_H)
check_include_file(fcntl.h      HAVE_FCNTL_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(setjmp.h     HAVE_SETJMP_H)
check_include_file(signal.h     HAVE_SIGNAL_H)
check_include_file(stdarg.h     HAVE_STDARG_H)
check_include_file(stddef.h     HAVE_STDDEF_H)
check_include_file(stdint.h     HAVE_STDINT_H)
check_include_file(stdlib.h     HAVE_STDLIB_H)
check_include_file(string.h     HAVE_STRING_H)
check_include_file(syslog.h     HAVE_SYSLOG_H)
check_include_file(sys/mman.h   HAVE_SYS_MMAN_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/stat.h   HAVE_SYS_STAT_H)
check_include_file(sys/types.h  HAVE_SYS_TYPES_H)
check_include_file(sys/wait.h   HAVE_SYS_WAIT_H)
check_include_file(time.h       HAVE_TIME_H)
check_include_file(unistd.h     HAVE_UNISTD_H)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(ADDR_SRCS
	addr/addr_from_str.c
	addr/addr_hton.c
	addr/addr_htos.c
	addr/addr_ntoh.c
	addr/addr_stoh.c
	addr/addr_to_str.c
)

set(BITMAP_SRCS
	bitmap/i2v32.c
	bitmap/i2v64.c
	bitmap/v2i32.c
	bitmap/v2i64.c
)


set(CHROOT_SRCS
	chroot/chroot_fd.c
	chroot/chroot_mkdirp.c
	chroot/chroot_secure_chdir.c
)

set(EXEC_SRCS
	exec/exec_fork.c
	exec/exec_fork_background.c
	exec/exec_fork_pipe.c
	exec/exec_replace.c
)

set(FLIST_SRCS
	flist/flist32_from_str.c
	flist/flist32_getkey.c
	flist/flist32_getval.c
	flist/flist32_to_str.c
	flist/flist64_from_str.c
	flist/flist64_getkey.c
	flist/flist64_getval.c
	flist/flist64_to_str.c
)

set(LOG_SRCS
	log/log_close.c
	log/log_init.c
	log/log_internal.c
)

set(MEM_SRCS
	mem/mem_alloc.c
	mem/mem_ccpy.c
	mem/mem_chr.c
	mem/mem_cmp.c
	mem/mem_cpy.c
	mem/mem_dup.c
	mem/mem_free.c
	mem/mem_idx.c
	mem/mem_realloc.c
	mem/mem_set.c
)

set(MISC_SRCS
	misc/copy_file.c
	misc/isdir.c
	misc/isfile.c
	misc/islink.c
	misc/ismount.c
	misc/ispath.c
	misc/mkdirnamep.c
	misc/mkdirp.c
	misc/readsymlink.c
	misc/runlink.c
)

set(OPEN_SRCS
	open/open_append.c
	open/open_excl.c
	open/open_read.c
	open/open_rw.c
	open/open_trunc.c
	open/open_write.c
)

set(PRINTF_SRCS
	printf/asprintf.c
	printf/dprintf.c
	printf/printf.c
	printf/snprintf.c
	printf/vasprintf.c
	printf/vdprintf.c
	printf/vprintf.c
	printf/vsnprintf.c
)

set(RTTI_SRCS
	rtti/array.c
	rtti/bool.c
	rtti/data.c
	rtti/float.c
	rtti/int.c
	rtti/internal.h
	rtti/pointer.c
	rtti/rtti.c
	rtti/string.c
	rtti/struct.c
)

set(SCANF_SRCS
	scanf/sscanf.c
	scanf/vsscanf.c
)

set(STR_SRCS
	str/str_check.c
	str/str_chr.c
	str/str_cmp.c
	str/str_cmpn.c
	str/str_cpy.c
	str/str_cpyn.c
	str/str_dup.c
	str/str_equal.c
	str/str_len.c
	str/str_path_basename.c
	str/str_path_concat.c
	str/str_path_dirname.c
	str/str_path_isabs.c
	str/str_path_isdot.c
	str/str_rchr.c
	str/str_read.c
	str/str_readfile.c
	str/str_readline.c
	str/str_str.c
	str/str_tolower.c
	str/str_toumax.c
	str/str_toupper.c
)

set(STRALLOC_SRCS
	stralloc/stralloc_catb.c
	stralloc/stralloc_cat.c
	stralloc/stralloc_catf.c
	stralloc/stralloc_catm.c
	stralloc/stralloc_cats.c
	stralloc/stralloc_cmp.c
	stralloc/stralloc_copyb.c
	stralloc/stralloc_copy.c
	stralloc/stralloc_copys.c
	stralloc/stralloc_finalize.c
	stralloc/stralloc_free.c
	stralloc/stralloc_init.c
	stralloc/stralloc_ready.c
	stralloc/stralloc_readyplus.c
	stralloc/stralloc_zero.c
)

set(STRTOK_SRCS
	strtok/strtok_append.c
	strtok/strtok_count.c
	strtok/strtok_delete.c
	strtok/strtok_free.c
	strtok/strtok_init_argv.c
	strtok/strtok_init_str.c
	strtok/strtok_next.c
	strtok/strtok_prev.c
	strtok/strtok_toargv.c
	strtok/strtok_tostr.c
)

set(TCP_SRCS
	tcp/tcp_connect.c
	tcp/tcp_listen.c
)

set(WHIRLPOOL_SRCS
	whirlpool/whirlpool_add.c
	whirlpool/whirlpool_digest.c
	whirlpool/whirlpool_finalize.c
	whirlpool/whirlpool_init.c
	whirlpool/whirlpool_tables.h
	whirlpool/whirlpool_transform.c
)

set(lucid_SRCS
	${ADDR_SRCS}
	${BITMAP_SRCS}
	base64.c
	${CHROOT_SRCS}
	${EXEC_SRCS}
	${FLIST_SRCS}
	${LOG_SRCS}
	${MEM_SRCS}
	${MISC_SRCS}
	${OPEN_SRCS}
	${PRINTF_SRCS}
	${RTTI_SRCS}
	${SCANF_SRCS}
	${STR_SRCS}
	${STRALLOC_SRCS}
	${STRTOK_SRCS}
	${TCP_SRCS}
	${WHIRLPOOL_SRCS}
)

add_library(ucid SHARED ${lucid_SRCS})
set_target_properties(ucid PROPERTIES VERSION "0.0.0" SOVERSION "0")

install(
	TARGETS ucid
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
