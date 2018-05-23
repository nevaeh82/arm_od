if(PROTOBUF_INCLUDE_DIR AND PROTOBUF_LIBRARY)
	set(PROTOBUF_FIND_QUIETLY TRUE)
endif(PROTOBUF_INCLUDE_DIR AND PROTOBUF_LIBRARY)

set(PROTOBUF_PATH protobuf)

find_library(PROTOBUF_LIBRARY
	NAMES
		protobuf-lite${BICYCLE_DEBUG_POSTFIX}
		libprotobuf-lite${BICYCLE_DEBUG_POSTFIX}
		libprotobuf-lite${BICYCLE_DEBUG_POSTFIX}.lib
		protobuf${BICYCLE_DEBUG_POSTFIX}
		libprotobuf${BICYCLE_DEBUG_POSTFIX}
	PATHS ${PROJECT_BASE_DIR}/redist/${PROTOBUF_PATH}/${FULL_QT_VERSION}/lib_${SPEC})

#find_library(PROTOBUF_LIBRARY protobuf${BICYCLE_DEBUG_POSTFIX}
#	NAMES protobuf libprotobuf libprotobuf.lib
#	PATHS ${PROJECT_BASE_DIR}/redist/protobuf/lib_${SPEC})

find_path(PROTOBUF_INCLUDE_DIR
	google/protobuf/service.h
	PATHS ${PROJECT_BASE_DIR}/redist/${PROTOBUF_PATH}/${FULL_QT_VERSION}/src
)

find_program(PROTOBUF_COMPILER protoc
		PATHS ${PROJECT_BASE_DIR}/redist/${PROTOBUF_PATH}/${FULL_QT_VERSION})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Protobuf DEFAULT_MSG PROTOBUF_LIBRARY PROTOBUF_INCLUDE_DIR)

MARK_AS_ADVANCED(
	PROTOBUF_INCLUDE_DIR
	PROTOBUF_LIBRARY
	PROTOBUF_COMPILER
)
