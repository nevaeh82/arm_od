project(${PROJECT_NAME_GLOBAL}Common)

list(APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib QXT)

add_lib_static(${PROJECT_NAME} ${deps})
