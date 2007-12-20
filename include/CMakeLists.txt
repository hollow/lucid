SET(lucid_HDRS
	base64.h
	bitmap.h
	char.h
	chroot.h
	error.h
	exec.h
	flist.h
	list.h
	log.h
	mem.h
	printf.h
	rtti.h
	rpc.h
	scanf.h
	str.h
	stralloc.h
	strtok.h
	uio.h
	whirlpool.h
)

INSTALL(FILES ${lucid_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/lucid)
