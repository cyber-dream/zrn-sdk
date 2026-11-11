cmake_minimum_required(VERSION 3.16)

project(zrnui VERSION 2.0.0 DESCRIPTION "zrnui - WP8.1 UI in C" LANGUAGES C)

# ON when zrnui is the project being built, OFF when it is added with add_subdirectory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(ZRNUI_IS_TOP_LEVEL ON)
else()
	set(ZRNUI_IS_TOP_LEVEL OFF)
endif()

option(ZRNUI_BUILD_SHARED "Build zrnui as a shared library" OFF)
option(ZRNUI_BUILD_TESTS "Build the tests" ${ZRNUI_IS_TOP_LEVEL})
# Z_DEBUG switches the debug features on, in the library and in every target that links it
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ZRNUI_DEBUG_DEFAULT ON)
else()
	set(ZRNUI_DEBUG_DEFAULT OFF)
endif()
option(ZRNUI_DEBUG "Define Z_DEBUG: build the debug features" ${ZRNUI_DEBUG_DEFAULT})
option(ZRNUI_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE REQUIRED IMPORTED_TARGET freetype2)
pkg_check_modules(PNG REQUIRED IMPORTED_TARGET libpng)
# Selawik is a Latin subset: Cyrillic and everything else is drawn with a font fontconfig finds for the character
pkg_check_modules(FONTCONFIG IMPORTED_TARGET fontconfig)

set(ZRNUI_SOURCES
	src/core.c
	src/anim.c
	src/theme.c
	src/theme_resources.c
	src/resources.c
	src/font.c
	src/text.c
	src/render/painter.c
	src/render/software.c
	src/render/gles.c
	src/host.c
	src/controls/panels.c
	src/controls/basic.c
	src/controls/buttons.c
	src/controls/range.c
	src/controls/scroll.c
	src/controls/textbox.c
	src/controls/items.c
	src/controls/listview.c
	src/controls/combo.c
	src/controls/flip.c
	src/controls/frame.c
	src/controls/pivot.c
	src/controls/hub.c
	src/controls/pickers.c
	src/controls/richtext.c
	src/controls/appbar.c
	src/controls/flyout.c
	src/platform.c
)

# the OpenGL ES 2 renderer: the window and Zroot_render use it when asked or when the machine has it;
# the software painter is always built and is the safe mode
option(ZRNUI_WITH_GL "Build the OpenGL ES 2 renderer when egl and glesv2 are found" ON)
if(ZRNUI_WITH_GL)
	pkg_check_modules(GLES IMPORTED_TARGET egl glesv2)
endif()

if(ZRNUI_BUILD_SHARED)
	add_library(zrnui SHARED ${ZRNUI_SOURCES})
	target_compile_definitions(zrnui PRIVATE ZRNUI_BUILD_DLL PUBLIC ZRNUI_USE_DLL)
	set_target_properties(zrnui PROPERTIES C_VISIBILITY_PRESET hidden)
else()
	add_library(zrnui STATIC ${ZRNUI_SOURCES})
endif()
add_library(zrnui::zrnui ALIAS zrnui)
if(ZRNUI_DEBUG)
	target_compile_definitions(zrnui PUBLIC Z_DEBUG)
endif()

# memfd_create, readlink, strtof and friends; defined here, not in the sources
target_compile_definitions(zrnui PRIVATE _GNU_SOURCE)
target_include_directories(zrnui PUBLIC include PRIVATE src)
target_link_libraries(zrnui PRIVATE PkgConfig::FREETYPE PkgConfig::PNG m)
if(GLES_FOUND)
	target_link_libraries(zrnui PRIVATE PkgConfig::GLES)
	target_compile_definitions(zrnui PRIVATE ZRNUI_HAVE_GL)
endif()
if(FONTCONFIG_FOUND)
	target_link_libraries(zrnui PRIVATE PkgConfig::FONTCONFIG)
	target_compile_definitions(zrnui PRIVATE ZRNUI_HAVE_FONTCONFIG)
else()
	message(WARNING "fontconfig not found: only the Latin letters of the bundled Selawik fonts will be drawn")
endif()

# the Selawik fonts that stand in for Segoe WP are found here unless Zset_font_directory / $ZRNUI_FONT_DIR says otherwise
set(ZRNUI_FONT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fonts/selawik" CACHE PATH "Directory containing the bundled Selawik fonts")
target_compile_definitions(zrnui PRIVATE ZRNUI_FONT_DIR="${ZRNUI_FONT_DIR}")

# the theme colors: the readable file, overridable with $ZRNUI_THEME
set(ZRNUI_THEME_FILE "${CMAKE_CURRENT_SOURCE_DIR}/theme/theme.ini" CACHE FILEPATH "Path to the theme.ini file")
target_compile_definitions(zrnui PRIVATE ZRNUI_THEME_FILE="${ZRNUI_THEME_FILE}")

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(zrnui PRIVATE -Wall -Wextra -Wno-unused-parameter)
	# C23 allows `const auto`; GCC still warns that the storage class specifier is not first
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		target_compile_options(zrnui PRIVATE -Wno-old-style-declaration)
	endif()
	if(ZRNUI_WARNINGS_AS_ERRORS)
		target_compile_options(zrnui PRIVATE -Werror)
	endif()
endif()

if(ZRNUI_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

# libFuzzer targets (clang only): cmake -DZRNUI_BUILD_FUZZERS=ON, then run fuzz/fuzz_text etc. on a corpus directory
option(ZRNUI_BUILD_FUZZERS "Build the libFuzzer targets (needs clang)" OFF)
if(ZRNUI_BUILD_FUZZERS)
	add_subdirectory(fuzz)
endif()

# coverage of the tests: cmake -DZRNUI_COVERAGE=ON, run ctest, then the `coverage` target prints a gcov summary
option(ZRNUI_COVERAGE "Build with gcov instrumentation" OFF)
if(ZRNUI_COVERAGE)
	target_compile_options(zrnui PRIVATE --coverage -O0)
	target_link_libraries(zrnui PUBLIC --coverage)
endif()
