SET(lucid_HDRS
	addr.h
	bitmap.h
	char.h
	chroot.h
	exec.h
	flist.h
	log.h
	mem.h
	misc.h
	open.h
	printf.h
	scanf.h
	str.h
	stralloc.h
	strtok.h
	tcp.h
	whirlpool.h
)

INSTALL(FILES ${lucid_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR})
