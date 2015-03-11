project(ZaviruhaOD)
cmake_minimum_required(VERSION 2.6.3)

if(NOT VERSION)
	set(VERSION "1.5.0")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(UsefulMacros)

# project options
option(MAKE_TESTS "Include tests in solution" ON)
option(INSTALL_RUNTIME_PATH "Install rpath" OFF)
option(INSTALL_RUNTIME_LIBS "Install runtime libs" ON)
option(USE_SYSLOG "Use syslog instead qxtlogger" OFF)
option(USE_COVERAGE "Run coverage for statistics" OFF)
option(CREATE_TR "Dynamical create qrc with qm files" ON)
option(CREATE_BUNDLE "Create bundle on install" ON)
option(USE_SHARED_OUTPUT "Place all build binaries on one common direcotory bin" ON)
option(NO_BACKTRACE "Build without backtrace" OFF)

# proccess options
if(NO_BACKTRACE)
	add_definitions(-DNO_BACKTRACE)
endif()

if(USE_SYSLOG)
	add_definitions(-DUSE_SYSLOG)
endif()

if(WIN32)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

if(USE_SHARED_OUTPUT)
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
endif()

# include qt package
find_package(Qt4 4.8.0 COMPONENTS QtCore QtNetwork QtSql QtGui QtSvg QtWebKit REQUIRED)

include(CommonInit)
include(CodeCoverage)

# include thrid party packages
find_package(Threads REQUIRED)
find_package(Qxt REQUIRED)
find_package(Protobuf REQUIRED)
find_package(QtSingleApplication REQUIRED)
find_package(PwGis REQUIRED)

# add settings to installation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/settings/ DESTINATION ${BINDIR})

# scripts for runtime libs installation and copying
if(INSTALL_RUNTIME_LIBS OR USE_SHARED_OUTPUT)
	set(runtime_libs)

	# collect runtime libs
	# todo: maybe it will be better to move the same actions below in function
	if(WIN32)
		if(QXT_LIBRARIES)
			foreach(lib ${QXT_LIBRARIES})
				get_filename_component(lib_dir ${lib} PATH)
				get_filename_component(lib_name ${lib} NAME_WE)
				string(REPLACE lib "" lib_name ${lib_name})
				file(GLOB libs ${lib_dir}/${lib_name}.dll)
				list(APPEND runtime_libs ${libs})
			endforeach()
		endif()

		if(PWGIS_LIBRARIES)
			foreach(lib ${PWGIS_LIBRARIES})
				get_filename_component(lib_dir ${lib} PATH)
				get_filename_component(lib_name ${lib} NAME_WE)
				string(REPLACE lib "" lib_name ${lib_name})
				file(GLOB libs ${lib_dir}/${lib_name}.dll)
				list(APPEND runtime_libs "${libs}")
			endforeach()
		endif()
	endif()

	# install runtime libs
	if(INSTALL_RUNTIME_LIBS)
		install(FILES ${runtime_libs} DESTINATION ${LIBDIR})
	endif()

	# copy runtime libs to shared output directory for better debugging
	if(USE_SHARED_OUTPUT)
		add_custom_target(copy_runtime ALL COMMENT "Copy runtime libs to shared output directy")

		foreach(lib ${runtime_libs})
			get_filename_component(lib_name ${lib} NAME)
			add_custom_command(
				TARGET copy_runtime POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different ${lib} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${lib_name})
		endforeach()

		# copy settings to shared output directory
		add_custom_command(
			TARGET copy_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/settings/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	endif()
endif()

add_subdirectory(src)

if(WIN32)
	include(nsis.cmake)

	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/win32/)
	set(CPACK_GENERATOR NSIS)

	set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
	set(CPACK_PACKAGE_VENDOR "STC")
	set(CPACK_PACKAGE_VERSION ${VERSION})

	# GUI output title
	set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME_RUS} ${VERSION}")

	# windows uninstaller display name
	set(CPACK_NSIS_DISPLAY_NAME "${CPACK_NSIS_PACKAGE_NAME}")

	# ???
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_NSIS_PACKAGE_NAME}")

	# set different file and registry settings
	set(CPACK_NSIS_INSTALL_ROOT "C:")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_VENDOR}\\\\${CPACK_PACKAGE_NAME}-${VERSION}")
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${VERSION}_Setup")
	set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${VERSION_MAJOR}.${VERSION_MINOR}")

	# enable uninstalling at first
	set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")

	# enable full delete of installation directory
	set(CPACK_NSIS_ENABLE_REMOVE_INSTDIR_RECURSIVE "ON")

	# add link to view app in Windows menu
	set(CPACK_NSIS_MENU_LINKS ${CPACK_NSIS_MENU_LINKS}
		"${PROJECT_NAME}View.exe" "${PROJECT_NAME_RUS} ${VERSION}")

	# set uninstall icon path
	set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}View.exe")

	# add create icon to view app in Windows Desktop
	set(CPACK_NSIS_CREATE_ICONS_EXTRA ${CPACK_NSIS_CREATE_ICONS_EXTRA}
		"\nCreateShortCut '$DESKTOP\\\\${PROJECT_NAME_RUS} ${VERSION}.lnk' '$INSTDIR\\\\${PROJECT_NAME}View.exe'\n")

	# add delete icon to view app in Windows Desktop
	set(CPACK_NSIS_DELETE_ICONS_EXTRA ${CPACK_NSIS_DELETE_ICONS_EXTRA}
		"\nDelete '$DESKTOP\\\\${PROJECT_NAME_RUS} ${VERSION}.lnk'\n")
endif()

include(CPack)
