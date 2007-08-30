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
	bitmap.c
	base64.c
	${CHROOT_SRCS}
	${EXEC_SRCS}
	${FLIST_SRCS}
	log.c
	mem.c
	printf.c
	${RTTI_SRCS}
	scanf.c
	str.c
	${STRALLOC_SRCS}
	${STRTOK_SRCS}
	uio.c
	${WHIRLPOOL_SRCS}
)

add_library(ucid SHARED ${lucid_SRCS})
set_target_properties(ucid PROPERTIES VERSION "0.0.0" SOVERSION "0")

install(
	TARGETS ucid
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
