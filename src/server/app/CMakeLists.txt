project(${PROJECT_NAME_GLOBAL}Server)

list(APPEND deps ${PROJECT_NAME_GLOBAL}ServerCore)

if(NOT NO_BACKTRACE)
	list(APPEND deps WIN32_BT)
endif()

add_service(${PROJECT_NAME} ${deps})

if(WIN32)
	# configure windows app version info
	configure_file(
		"${PROJECT_SOURCE_DIR}/version.h.in"
		"${PROJECT_BINARY_DIR}/version.h")

	# install batch for run server
	install(FILES ${PROJECT_SOURCE_DIR}/Server.bat DESTINATION ${BINDIR})

	if(INSTALL_RUNTIME_LIBS)
		# install Qt libs
		if(CREATE_BUNDLE)
			include(DeployQt4)
			install_qt4_executable(${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
		endif()
	endif()
endif()
