include(CPack)

project(lucid)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

set(EXEC_INSTALL_PREFIX  "${CMAKE_INSTALL_PREFIX}")
set(BIN_INSTALL_DIR      "${EXEC_INSTALL_PREFIX}/bin")
set(SBIN_INSTALL_DIR     "${EXEC_INSTALL_PREFIX}/sbin")
set(LIB_INSTALL_DIR      "${EXEC_INSTALL_PREFIX}/lib")
set(LIBEXEC_INSTALL_DIR  "${EXEC_INSTALL_PREFIX}/libexec")
set(INCLUDE_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/include")
set(SHARE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share")

add_definitions($ENV{CFLAGS} -D_GNU_SOURCE -D_LUCID_BUILD_)
add_definitions(
	-std=gnu99
	-pedantic
	-Werror
	-Wall
	-Wundef
	-Wcast-align
	-Wpointer-arith
	-Wwrite-strings
	-Winline
	-Wredundant-decls
	-Wno-unused-parameter
)

option(DEBUG "enable debugging flags" OFF)
if (DEBUG)
	add_definitions(-g -ggdb3)
endif(DEBUG)

enable_testing()

add_subdirectory(doc)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

configure_file(${CMAKE_SOURCE_DIR}/CPackSourceConfig.cmake
	${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake)
