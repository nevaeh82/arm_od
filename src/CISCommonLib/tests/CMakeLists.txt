project(${PROJECT_NAME_GLOBAL}CISCommonLibTest)

list(APPEND deps ${PROJECT_NAME_GLOBAL}CISCommonLib)

list(APPEND mock_interfaces
	${${PROJECT_NAME_GLOBAL}CISCommonLib_SOURCE_DIR}/Rpc/RpcPeerAbstract.h
	${${PROJECT_NAME_GLOBAL}CISCommonLib_SOURCE_DIR}/Interfaces/IRpcListener.h
)

set(need_mock_gen 1)
set(mock_gen ${PROJECT_BINARY_DIR}/Mocks.generated.h)

add_bicycle_test(${PROJECT_NAME} ${deps})
