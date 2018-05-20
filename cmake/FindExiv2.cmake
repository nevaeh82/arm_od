if(EXIV2_INCLUDE_DIR AND EXIV2_LIBRARY)
	set(EXIV2_FIND_QUIETLY TRUE)
endif(EXIV2_INCLUDE_DIR AND EXIV2_LIBRARY)

find_library(EXIV2_LIBRARY exiv2
	PATHS ${PROJECT_BASE_DIR}/redist/libexiv2/${FULL_QT_VERSION}/lib_${SPEC})

find_path(EXIV2_INCLUDE_DIR exiv2/exif.hpp
	PATHS ${PROJECT_BASE_DIR}/redist/libexiv2/${FULL_QT_VERSION}/include)

if (WIN32)
	set(EXIV2_LIBRARY ${EXIV2_LIBRARY} -lz -liconv -lpsapi)
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EXIV2 DEFAULT_MSG EXIV2_LIBRARY EXIV2_INCLUDE_DIR)

MARK_AS_ADVANCED(
	EXIV2_INCLUDE_DIR
	EXIV2_LIBRARY
)
