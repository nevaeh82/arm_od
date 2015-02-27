project(PrmSimulator)

list(APPEND deps
	${PROJECT_NAME_GLOBAL}CISCommonLib
	QXT
	${PROJECT_NAME_GLOBAL}Common
)

add_service(${PROJECT_NAME} ${deps})
