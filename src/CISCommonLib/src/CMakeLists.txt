project(${PROJECT_NAME_GLOBAL}CISCommonLib)

list(APPEND deps QXT)

#ADD_LIBRARY(${PROJECT_NAME} ${deps} STATIC IMPORTED)
add_lib_static(${PROJECT_NAME} ${deps})
