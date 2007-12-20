include(CheckIncludeFile)

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

set(RTTI_SRCS
	rtti/bool.c
	rtti/data.c
	rtti/flist.c
	rtti/inaddr.c
	rtti/int.c
	rtti/internal.h
	rtti/list.c
	rtti/pointer.c
	rtti/rtti.c
	rtti/string.c
	rtti/struct.c
	rtti/types.c
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
	bitmap.c
	base64.c
	cext.c
	${CHROOT_SRCS}
	error.c
	${EXEC_SRCS}
	flist.c
	log.c
	printf.c
	${RTTI_SRCS}
	rpc.c
	scanf.c
	str.c
	stralloc.c
	strtok.c
	uio.c
	${WHIRLPOOL_SRCS}
)

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

# libffi
find_path(FFI_INCLUDE_DIR ffi.h /usr/include/libffi /usr/local/include/libffi)
find_library(FFI_LIBRARY NAMES ffi PATHS libffi)

if (FFI_INCLUDE_DIR AND FFI_LIBRARY)
	message(STATUS "Found libffi: ${FFI_LIBRARY}")
else (FFI_INCLUDE_DIR AND FFI_LIBRARY)
	message(FATAL_ERROR "Could not find libffi")
endif(FFI_INCLUDE_DIR AND FFI_LIBRARY)

# libdl
find_library(DL_LIBRARY dl)
if (DL_LIBRARY)
	message(STATUS "Found libdl: ${DL_LIBRARY}")
else (DL_LIBRARY)
	message(FATAL_ERROR "Could not find libdl")
endif(DL_LIBRARY)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${FFI_INCLUDE_DIR})

add_library(ucid SHARED ${lucid_SRCS})
set_target_properties(ucid PROPERTIES VERSION "0.0.0" SOVERSION "0")
target_link_libraries(ucid ${FFI_LIBRARY} ${DL_LIBRARY})

install(
	TARGETS ucid
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
