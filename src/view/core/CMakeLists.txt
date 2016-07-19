project(${PROJECT_NAME_GLOBAL}viewCore)

list( APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib
                   QXT
                   PWGIS
                   ${PROJECT_NAME_GLOBAL}Common)

add_lib_static(${PROJECT_NAME} ${deps})
