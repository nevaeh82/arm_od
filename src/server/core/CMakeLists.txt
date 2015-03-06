project(${PROJECT_NAME_GLOBAL}ServerCore)

list(APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib QXT PROTOBUF QTSINGLEAPPLICATION ${PROJECT_NAME_GLOBAL}Common)

file(GLOB protofiles protobuf/*.proto)
if(UNIX)
install(FILES ${protofiles} DESTINATION ${CLIENT_DATA_DIR}/proto)
else()
install(FILES ${protofiles} DESTINATION Protobuf)
endif()

add_lib_static(${PROJECT_NAME} ${deps})
