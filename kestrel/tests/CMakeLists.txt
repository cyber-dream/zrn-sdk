# one executable per file
set(KESTREL_TESTS test_layout test_controls test_input test_render test_navigation test_gpu test_animation test_pivot test_items test_bars test_flyouts test_hub test_pickers test_richtext test_resources)

foreach(name ${KESTREL_TESTS})
	add_executable(${name} ${name}.c)
	target_link_libraries(${name} PRIVATE kestrel PkgConfig::PNG m)
	target_compile_definitions(${name} PRIVATE KESTREL_TEST_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}")
	if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		target_compile_options(${name} PRIVATE -Wall -Wextra -Wno-unused-parameter)
	endif()
	add_test(NAME ${name} COMMAND ${name})
endforeach()

# test_resources reads the internal color table to check that it agrees with the generated one
target_include_directories(test_resources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# the EGL driver leaks a little of its own at exit, which LeakSanitizer would report as ours
set_tests_properties(test_gpu test_animation PROPERTIES ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
