SET(lucid_HDRS
	addr.h
	base64.h
	bitmap.h
	char.h
	chroot.h
	exec.h
	flist.h
	list.h
	log.h
	mem.h
	printf.h
	rtti.h
	scanf.h
	str.h
	stralloc.h
	strtok.h
	uio.h
	whirlpool.h
)

INSTALL(FILES ${lucid_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/lucid)
