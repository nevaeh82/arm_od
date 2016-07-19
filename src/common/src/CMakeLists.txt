project(${PROJECT_NAME_GLOBAL}Common)

list(APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib
                QXT
                Protobuf)

file(GLOB protofiles protobuf/*.proto)

add_lib_static(${PROJECT_NAME} ${deps})
