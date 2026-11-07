cmake_minimum_required(VERSION 3.16)

project(kestrel VERSION 0.1.0 DESCRIPTION "Kestrel - Windows Phone 8.1 XAML controls in C" LANGUAGES C)

# ON when Kestrel is the project being built, OFF when it is added with add_subdirectory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(KESTREL_IS_TOP_LEVEL ON)
else()
	set(KESTREL_IS_TOP_LEVEL OFF)
endif()

option(KESTREL_BUILD_SHARED "Build Kestrel as a shared library" OFF)
option(KESTREL_BUILD_TESTS "Build the tests" ${KESTREL_IS_TOP_LEVEL})
option(KESTREL_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FREETYPE REQUIRED IMPORTED_TARGET freetype2)
pkg_check_modules(PNG REQUIRED IMPORTED_TARGET libpng)
# Selawik is a Latin subset: Cyrillic and everything else is drawn with a font fontconfig finds for the character
pkg_check_modules(FONTCONFIG IMPORTED_TARGET fontconfig)

set(KESTREL_SOURCES
	src/core.c
	src/anim.c
	src/theme.c
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
	src/window.c
)

# the OpenGL ES 2 renderer: the window and Kroot_render use it when asked or when the machine has it;
# the software painter is always built and is the safe mode
option(KESTREL_WITH_GL "Build the OpenGL ES 2 renderer when egl and glesv2 are found" ON)
if(KESTREL_WITH_GL)
	pkg_check_modules(GLES IMPORTED_TARGET egl glesv2)
endif()

# a Wayland window (wl_shm, no GPU); without these libraries Kwindow_run fails at run time
option(KESTREL_WITH_WAYLAND "Build the Wayland window (Kwindow_run) when wayland-client and xkbcommon are found" ON)
if(KESTREL_WITH_WAYLAND)
	pkg_check_modules(WAYLAND IMPORTED_TARGET wayland-client xkbcommon)
endif()
if(WAYLAND_FOUND)
	list(APPEND KESTREL_SOURCES protocol/xdg-shell-protocol.c)
endif()

if(KESTREL_BUILD_SHARED)
	add_library(kestrel SHARED ${KESTREL_SOURCES})
	target_compile_definitions(kestrel PRIVATE KESTREL_BUILD_DLL PUBLIC KESTREL_USE_DLL)
	set_target_properties(kestrel PROPERTIES C_VISIBILITY_PRESET hidden)
else()
	add_library(kestrel STATIC ${KESTREL_SOURCES})
endif()
add_library(kestrel::kestrel ALIAS kestrel)

target_include_directories(kestrel PUBLIC include PRIVATE src)
target_link_libraries(kestrel PRIVATE PkgConfig::FREETYPE PkgConfig::PNG m)
if(WAYLAND_FOUND)
	target_link_libraries(kestrel PRIVATE PkgConfig::WAYLAND)
	target_compile_definitions(kestrel PRIVATE KESTREL_HAVE_WAYLAND)
endif()
if(GLES_FOUND)
	target_link_libraries(kestrel PRIVATE PkgConfig::GLES)
	target_compile_definitions(kestrel PRIVATE KESTREL_HAVE_GL)
	if(WAYLAND_FOUND)
		pkg_check_modules(WAYLAND_EGL IMPORTED_TARGET wayland-egl)
		if(WAYLAND_EGL_FOUND)
			target_link_libraries(kestrel PRIVATE PkgConfig::WAYLAND_EGL)
		endif()
	endif()
endif()
if(FONTCONFIG_FOUND)
	target_link_libraries(kestrel PRIVATE PkgConfig::FONTCONFIG)
	target_compile_definitions(kestrel PRIVATE KESTREL_HAVE_FONTCONFIG)
else()
	message(WARNING "fontconfig not found: only the Latin letters of the bundled Selawik fonts will be drawn")
endif()

# the Selawik fonts that stand in for Segoe WP are found here unless Kset_font_directory / $KESTREL_FONT_DIR says otherwise
target_compile_definitions(kestrel PRIVATE KESTREL_FONT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fonts")

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(kestrel PRIVATE -Wall -Wextra -Wno-unused-parameter)
	if(KESTREL_WARNINGS_AS_ERRORS)
		target_compile_options(kestrel PRIVATE -Werror)
	endif()
endif()

if(KESTREL_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
