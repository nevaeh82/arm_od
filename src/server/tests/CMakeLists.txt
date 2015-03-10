project(${PROJECT_NAME_GLOBAL}ServerCoreTest)

list(APPEND deps ${PROJECT_NAME_GLOBAL}ServerCore)

add_bicycle_test(${PROJECT_NAME} ${deps})
