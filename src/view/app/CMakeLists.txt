project(${PROJECT_NAME_GLOBAL}view)

#MESSAGE(${MYSQL_LIB})
list(APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib QXT PWGIS ${PROJECT_NAME_GLOBAL}Common ${PROJECT_NAME_GLOBAL}viewCore)

add_service(${PROJECT_NAME} ${deps})

#INSTALLING QT LIBS
			if(SPEC STREQUAL "mingw")
				set(gp_tool "objdump")
			endif()
			include(DeployQt4)
			install_qt4_executable(${PROJECT_NAME_GLOBAL}view${CMAKE_EXECUTABLE_SUFFIX})
