project(${PROJECT_NAME_GLOBAL}-src)

add_subdirectory(CISCommonLib)
add_subdirectory(ARM_OD_Common)

add_subdirectory(ARM_OD_Server)
add_subdirectory(ARM_OD_ServerCore)
add_subdirectory(ARM_OD_view)
add_subdirectory(ARM_OD_viewCore)

add_subdirectory(KtrSimulator)
add_subdirectory(PrmSimulator)

if(MAKE_TESTS)
add_subdirectory(ARM_OD_ServerCoreTest)
add_subdirectory(ARM_OD_viewCoreTest)
add_subdirectory(TestShared)
endif()
