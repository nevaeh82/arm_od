project(${PROJECT_NAME_GLOBAL}ServerCoreTest)

list(APPEND deps QXT PROTOBUF ${PROJECT_NAME_GLOBAL}CISCommonLib ${PROJECT_NAME_GLOBAL}Common ${PROJECT_NAME_GLOBAL}ServerCore )

add_bicycle_test(${PROJECT_NAME} ${deps})
