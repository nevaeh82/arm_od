enable_testing()

set(PROJECT_NAME_GLOBAL ${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOW)

if(NOT SPEC OR NOT HOST)
	if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		set(HOST linux)
		if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
			set(SPEC linux)
		elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7l")
			set(SPEC arm)
		elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
			set(SPEC astra)
		endif()
	elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		set(HOST win32)
		if(MSVC)
			set(SPEC msvc)
		else()
			set(SPEC mingw)
		endif()
	endif()
endif()

set(MOC_ARGS "-nn")
set(CMAKE_AUTOMOC_MOC_OPTIONS "-nn")

if(HOST STREQUAL "linux")
	if(NOT PYTHON)
		set(PYTHON "python2")
	endif()
else()
	set(PYTHON "python")
endif()

message(STATUS "PYTHON: ${PYTHON}")
message(STATUS "MOC_ARGS: ${MOC_ARGS}")

set(PROJECT_BASE_DIR ${PROJECT_SOURCE_DIR})

if(LIB_INSTALL_DIR)
	set(LIBDIR ${LIB_INSTALL_DIR})
endif(LIB_INSTALL_DIR)

if(NOT LIBDIR)
	if(WIN32)
		set(LIBDIR .)
	else(WIN32)
		set(LIBDIR lib/${PROJECT_NAME_LOW})
	endif(WIN32)
endif(NOT LIBDIR)

if(NOT BINDIR)
	if(WIN32)
		set(BINDIR .)
	elseif(UNIX)
		set(BINDIR bin)
		if(APPLE)
			set(BUNDLEDIR .)
		endif(APPLE)
	endif(WIN32)
endif()

message(STATUS "Library directory: ${CMAKE_INSTALL_PREFIX}/${LIBDIR}")

if(INSTALL_RUNTIME_PATH)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIBDIR};./")
endif(INSTALL_RUNTIME_PATH)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif(NOT CMAKE_BUILD_TYPE)

string( TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPEUP)

if(CMAKE_BUILD_TYPEUP STREQUAL DEBUG AND WIN32)
	set(BICYCLE_DEBUG_POSTFIX "d")
endif()

if(NOT SHARE_DIR)
	set(SHARE_DIR "share")
endif(NOT SHARE_DIR)

if(APPLE)
	add_definitions(-DAPPLE)
endif(APPLE)

if(NOT CLIENT_DATA_DIR)
	if(APPLE)
		set(CLIENT_DATA_DIR "${PROJECT_NAME_GLOBAL}.app")
	elseif(WIN32 OR HAIKU)
		set(CLIENT_DATA_DIR "${SHARE_DIR}")
	else(APPLE)
		set(CLIENT_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${SHARE_DIR}/${PROJECT_NAME_LOW}")
	endif(APPLE)
endif(NOT CLIENT_DATA_DIR)

#message(STATUS "Data directory: ${CLIENT_DATA_DIR}")

if(REVISION)
	set(WC_REVISION ${REVISION})
	message(STATUS "REVISION is set to: ${WC_REVISION}")
elseif(GIT_FOUND)
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
				OUTPUT_VARIABLE WC_REVISION
				OUTPUT_STRIP_TRAILING_WHITESPACE)
	message(STATUS "Build Git commit ID: ${WC_REVISION}")
	if(NOT WC_REVISION MATCHES [0-9a-z]+)
		set(WC_REVISION "${VERSION_SUFFIX}")
	endif()
elseif(NOT REVISION)
	set(WC_REVISION "${VERSION_SUFFIX}")
endif()

if( VERSION )
	string( REGEX REPLACE "\\..+" "" VERSION_MAJOR ${VERSION} )
	string( REGEX REPLACE "^[^.]+\\.([^.]+).*" "\\1" VERSION_MINOR ${VERSION} )
	string( REGEX REPLACE "^[^.]+\\.[^.]+\\.([^.]+).*" "\\1" VERSION_PATCH ${VERSION} )
endif()

if(UNIX)
add_definitions(-D_GNU_SOURCE)
endif(UNIX)

add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_DATADIR="${CLIENT_DATA_DIR}/")

if(CMAKE_CXX_COMPILER_ID STREQUAL GNU OR CMAKE_CXX_COMPILER_ID STREQUAL Clang)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pipe -Wformat -Werror=format-security -Wl,--as-needed -Wall -Wextra -Werror=return-type")
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
	endif()
endif()

if(USE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
	 include(CodeCoverage)
endif()

option(LANGUAGES "Using translations")
list(APPEND AllLanguages en ru)
if(NOT DEFINED linguas)
	set(LANGUAGES ${AllLanguages} CACHE STRING "Using translations" FORCE)
else(NOT DEFINED linguas)
	if(NOT linguas)
	set(LANGUAGES "" CACHE STRING "Using translations" FORCE)
	elseif(linguas STREQUAL *)
		set(LANGUAGES ${AllLanguages} CACHE STRING "Using translations" FORCE)
	else(NOT linguas)
	string(REGEX MATCHALL [a-zA-Z_@]+
			 linguas1 ${linguas})
	set(LANGUAGES ${linguas1} CACHE STRING "Using translations" FORCE)
	endif(NOT linguas)
endif(NOT DEFINED linguas)
message(STATUS "Translations: ${LANGUAGES}")

message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)
add_custom_target(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# try to get qt version (mostly for qt 5)
if( NOT QT_VERSION_MAJOR OR NOT QT_VERSION_MINOR )
	execute_process( COMMAND qmake -query QT_VERSION OUTPUT_VARIABLE QT_VERSION )

	string( REGEX REPLACE "\\..+" "" QT_VERSION_MAJOR "${QT_VERSION}" )
	string( REGEX REPLACE "^[^.]+\\.([^.]+).*" "\\1" QT_VERSION_MINOR "${QT_VERSION}" )
endif()

set(QT_VERSION_API ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR})
set(FULL_QT_VERSION ${QT_VERSION_API})
