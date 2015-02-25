project(ARM_OD)
cmake_minimum_required(VERSION 2.6.3)

set(PROJECT_NAME_GLOBAL ${PROJECT_NAME})

set(PROJECT_NAME "ARM_OD")
set(PROJECT_NAME_LOW "ARM_OD")

if(NOT ARMVERSION)
	set(ARMVERSION "1.0")
endif()



@#if(WIN32)
#set(WIN32_BT_LIBRARIES "-lbfd -lintl -liberty -limagehlp")
#endif()

set(VERSION_SUFFIX "GIT")

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(FindPkgConfig)
include(CheckCXXSourceCompiles)
find_package(PkgConfig)
find_package(Git)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(UsefulMacros)

option(MAKE_TESTS "Make Tests" ON)
option(INSTALL_RUNTIME_PATH "Install rpath" OFF)
option(INSTALL_RUNTIME_LIBS "Install runtime libs" OFF)
option(NO_PWGIS "Build without PwGis" OFF)
option(USE_SYSLOG "Use syslog instead qxtlogger" OFF)
option(USE_COVERAGE "Run coverage for statistics" OFF)
option(CREATE_TR "Dynamical create qrc with qm files" ON)
option(CREATE_BUNDLE "Create bundle on install" OFF)
option(USE_SHARED_OUTPUT "Place all build binaries on one common direcotory bin" OFF)

option(NO_BACKTRACE "Build without backtrace" OFF)
if(NO_BACKTRACE)
add_definitions(-DNO_BACKTRACE)
endif()

include(CommonInit)
find_package(Threads REQUIRED)

if(USE_SHARED_OUTPUT)
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
endif()

if(USE_SYSLOG)
add_definitions(-DUSE_SYSLOG)
endif()

find_package(Qt4 4.7.0 COMPONENTS QtCore QtNetwork QtSql REQUIRED)

set(FULL_QT_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

find_package(Qxt REQUIRED)
find_package(Protobuf REQUIRED)
find_package(QtSingleApplication REQUIRED)

if(NOT NO_PWGIS)
find_package(Qt4 4.7.0 COMPONENTS QtCore QtNetwork QtSql QtGui QtOpenGl QtSvg QtWebkit REQUIRED)
find_package(PwGis REQUIRED)
endif()

endif()


configure_file(
	"${PROJECT_SOURCE_DIR}/version.ini.in"
	"${PROJECT_BINARY_DIR}/version.ini")

configure_file(
	"${PROJECT_SOURCE_DIR}/Version.h.in"
	"${PROJECT_BINARY_DIR}/VersionGlobal.h")

if( ARMVERSION )
		string( REGEX REPLACE "\\..+" "" ARMVERSION_MAJOR ${ARMVERSION} )
		string( REGEX REPLACE "^[^.]+\\.([^.]+).*" "\\1" ARMVERSION_MINOR ${ARMVERSION} )
		string( REGEX REPLACE "^[^.]+\\.[^.]+\\.([^.]+).*" "\\1" ARMVERSION_PATCH ${ARMVERSION} )
endif()

include_directories(${PROJECT_BINARY_DIR})

install(FILES ${PROJECT_BINARY_DIR}/version.ini DESTINATION ${CLIENT_DATA_DIR})

set(NSIS_EXE_FOR_LINKS
	""
	CACHE STRING "" FORCE)

add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/win32/)
set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_VENDOR "STC")
set(CPACK_PACKAGE_VERSION ${ARMVERSION})
set(CPACK_NSIS_INSTALL_ROOT "C:")

set(CPACK_PACKAGE_NAME "ARM_OD")
set(CPACK_NSIS_INSTALLED_ICON_NAME "ARM_OD.exe")
list(APPEND CPACK_NSIS_CREATE_ICONS_EXTRA "
CreateShortCut '$DESKTOP\\\\ARM_OD-${ARMVERSION}.lnk' '$INSTDIR\\\\ARM_OD.exe'
")
list(APPEND CPACK_NSIS_DELETE_ICONS_EXTRA "
Delete '$DESKTOP\\\\ARM_OD-${ARMVERSION}.lnk'
")

set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME} ${ARMVERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME} ${ARMVERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}Setup_${ARMVERSION}")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${CPACK_NSIS_DISPLAY_NAME})

unset(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
unset(NSIS_MENU_LINKS)

set(inst_nsis_extra_path "${CMAKE_CURRENT_SOURCE_DIR}/win32/inst_extra.nsh")
string(REPLACE "/" "\\\\" inst_nsis_extra_path_out ${inst_nsis_extra_path})

set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "!include ${inst_nsis_extra_path_out}")

set(uninst_nsis_extra_path "${CMAKE_CURRENT_SOURCE_DIR}/win32/uninst_extra.nsh")
string(REPLACE "/" "\\\\" uninst_nsis_extra_path_out ${uninst_nsis_extra_path})

set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
"!include ${uninst_nsis_extra_path_out}
")

foreach(NSIS_EXE_FOR_LINK ${NSIS_EXE_FOR_LINKS})
set(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS} "${NSIS_EXE_FOR_LINK}.exe" "${NSIS_EXE_FOR_LINK}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win32/uninst_kill_process_extra.nsh.in ${PROJECT_BINARY_DIR}/uninst_kill_process_extra.nsh.${NSIS_EXE_FOR_LINK} @ONLY)
string(REPLACE "/" "\\\\" uninst_nsis_kill_extra_path_out ${PROJECT_BINARY_DIR}/uninst_kill_process_extra.nsh.${NSIS_EXE_FOR_LINK})
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
"${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
!include ${uninst_nsis_kill_extra_path_out}
")
endforeach()
endif()

if(INSTALL_RUNTIME_LIBS)
	if(WIN32)
			if(QXT_LIBRARIES)
			foreach(qxt_lib ${QXT_LIBRARIES})
					get_filename_component(QXT_LIBRARY_DIR ${qxt_lib} PATH)
					get_filename_component(QXT_LIBRARY_NAME ${qxt_lib} NAME_WE)
					string(REPLACE lib "" QXT_LIBRARY_NAME_NEW ${QXT_LIBRARY_NAME})
					file(GLOB qxt_libs ${QXT_LIBRARY_DIR}/${QXT_LIBRARY_NAME_NEW}.dll)
					install(FILES ${qxt_libs} DESTINATION ${LIBDIR})
			endforeach()
			endif()
			if(PWGIS_LIBRARIES)
				foreach(pwgis_lib ${PWGIS_LIBRARIES})
					get_filename_component(PWGIS_LIBRARY_DIR ${pwgis_lib} PATH)
					get_filename_component(PWGIS_LIBRARY_NAME ${pwgis_lib} NAME_WE)
					string(REPLACE lib "" PWGIS_LIBRARY_NAME_N ${PWGIS_LIBRARY_NAME})

					file(GLOB pwgis_libs1 ${PWGIS_LIBRARY_DIR}/${PWGIS_LIBRARY_NAME_N}1.dll)
					file(GLOB pwgis_libs2 ${PWGIS_LIBRARY_DIR}/${PWGIS_LIBRARY_NAME_N}.dll)
					set(pwgis_libs "${pwgis_libs1};${pwgis_libs2}")
					if( NOT ${pwgis_libs} STREQUAL "" )
						install(FILES ${pwgis_libs} DESTINATION ${LIBDIR})
					else()
						install(FILES ${pwgis_lib} DESTINATION ${LIBDIR})
					endif()
				endforeach()
			endif()
	endif()

endif()

include(CPack)