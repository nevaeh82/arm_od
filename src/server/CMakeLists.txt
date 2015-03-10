add_subdirectory(core)
add_subdirectory(app)

if(MAKE_TESTS)
	add_subdirectory(tests)
endif()