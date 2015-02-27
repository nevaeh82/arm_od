project(KtrEmulator)

list(APPEND deps
	${PROJECT_NAME_GLOBAL}CISCommonLib
)

add_service(${PROJECT_NAME} ${deps})