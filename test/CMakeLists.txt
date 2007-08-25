include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(addr addr.c)
target_link_libraries(addr ucid)
add_test(addr_test addr)

add_executable(bitmap bitmap.c)
target_link_libraries(bitmap ucid)
add_test(bitmap ./bitmap)

add_executable(chroot chroot.c)
target_link_libraries(chroot ucid)
add_test(chroot ./chroot)

add_executable(flist flist.c)
target_link_libraries(flist ucid)
add_test(flist ./flist)

add_executable(str str.c)
target_link_libraries(str ucid)
add_test(str ./str)

add_executable(whirlpool whirlpool.c)
target_link_libraries(whirlpool ucid)
add_test(whirlpool ./whirlpool)
