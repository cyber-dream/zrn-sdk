# one libFuzzer executable per file; the library is built with the same sanitizers
if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
	message(FATAL_ERROR "ZRNUI_BUILD_FUZZERS needs clang")
endif()

target_compile_options(zrnui PRIVATE -fsanitize=fuzzer-no-link,address,undefined -g)
target_link_options(zrnui PUBLIC -fsanitize=address,undefined)

foreach(name fuzz_text fuzz_png fuzz_theme)
	add_executable(${name} ${name}.c)
	target_compile_options(${name} PRIVATE -fsanitize=fuzzer,address,undefined -g)
	target_link_options(${name} PRIVATE -fsanitize=fuzzer,address,undefined)
	target_link_libraries(${name} PRIVATE zrnui)
endforeach()
