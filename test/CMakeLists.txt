include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(base64 base64.c)
target_link_libraries(base64 ucid)
add_test(base64 base64)

add_executable(bitmap bitmap.c)
target_link_libraries(bitmap ucid)
add_test(bitmap bitmap)

add_executable(chroot chroot.c)
target_link_libraries(chroot ucid)
add_test(chroot chroot)

add_executable(flist flist.c)
target_link_libraries(flist ucid)
add_test(flist flist)

#add_executable(rtti rtti.c)
#target_link_libraries(rtti ucid)
#add_test(rtti rtti)

add_executable(str str.c)
target_link_libraries(str ucid)
add_test(str str)

add_executable(whirlpool whirlpool.c)
target_link_libraries(whirlpool ucid)
add_test(whirlpool whirlpool)
