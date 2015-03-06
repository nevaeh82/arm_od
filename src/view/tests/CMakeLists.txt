project(${PROJECT_NAME_GLOBAL}viewCoreTest)

list(APPEND deps QXT PWGIS ${PROJECT_NAME_GLOBAL}CISCommonLib ${PROJECT_NAME_GLOBAL}Common ${PROJECT_NAME_GLOBAL}viewCore )

add_bicycle_test(${PROJECT_NAME} ${deps})
