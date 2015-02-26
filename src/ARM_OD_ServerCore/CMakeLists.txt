project(${PROJECT_NAME_GLOBAL}ServerCore)

list(APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib QXT PROTOBUF QTSINGLEAPPLICATION ${PROJECT_NAME_GLOBAL}Common)


add_lib_static(${PROJECT_NAME} ${deps})

message("ServerCoretest message:: ${libs} ")
