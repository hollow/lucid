set(CPACK_CMAKE_GENERATOR          "Unix Makefiles")
set(CPACK_GENERATOR                "TGZ;TBZ2")
set(CPACK_INSTALLED_DIRECTORIES    "${CMAKE_SOURCE_DIR};/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
set(CPACK_PACKAGE_NAME             "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION          "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME        "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_IGNORE_FILES "/CPack.*\\.cmake;/mkrelease;/build/;/CVS/;/\\.svn/;\\.swp$;\\.#;/#")
