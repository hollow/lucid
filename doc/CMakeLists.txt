find_package(Doxygen)

if(DOXYGEN_EXECUTABLE)
	option(ENABLE_DOXYGEN_DOCS "build API documentation (requires Doxygen)" ON)
endif(DOXYGEN_EXECUTABLE)

if (ENABLE_DOXYGEN_DOCS)
	configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.cmake
		${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
endif (ENABLE_DOXYGEN_DOCS)
