project(${PROJECT_NAME_GLOBAL}view)

list(APPEND deps ${PROJECT_NAME_GLOBAL}viewCore)

if(NOT NO_BACKTRACE)
	list(APPEND deps WIN32_BT)
endif()

add_service(${PROJECT_NAME} ${deps})

# copy runtime data to shared output dir
if(USE_SHARED_OUTPUT)
	add_custom_target(${PROJECT_NAME}_copy_runtime ALL COMMENT "Copy runtime libs to shared output directy for ${PROJECT_NAME}")

	add_custom_command(
		TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/qt.conf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qt.conf)
endif()

if(WIN32)
	# configure windows app version info
	configure_file(
		"${PROJECT_SOURCE_DIR}/version.h.in"
		"${PROJECT_BINARY_DIR}/version.h")

	# get plugins spec suffix
	if(MSVC)
		set(plugins_spec "-${SPEC}")
	endif()

	# init required redist pathes
	set(qt_plugins_dir ${PROJECT_BASE_DIR}/redist/plugins/${QT_VERSION_API}${plugins_spec})
	set(libmysql_dll ${PROJECT_BASE_DIR}/redist/libmysql/libmysql.dll)

	# install batch for run client and both (server+client)
	install(FILES ${PROJECT_SOURCE_DIR}/View.bat DESTINATION ${BINDIR})
	install(FILES ${PROJECT_SOURCE_DIR}/Platform.bat DESTINATION ${BINDIR})

	# if we should install runtime libs
	if(INSTALL_RUNTIME_LIBS)
		# install Qt plugins
		install(DIRECTORY ${qt_plugins_dir}/sqldrivers DESTINATION ${BINDIR}/plugins)
		install(FILES ${libmysql_dll} DESTINATION ${BINDIR})

		# install Qt libs
		if(CREATE_BUNDLE)
			include(DeployQt4)
			install_qt4_executable(${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
		endif()
	endif()

	# copy libs to runtime dir
	if(USE_SHARED_OUTPUT)
		add_custom_command(
			TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${qt_plugins_dir} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)

		add_custom_command(
			TARGET ${PROJECT_NAME}_copy_runtime POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${libmysql_dll} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libmysql.dll)
	endif()
endif()
