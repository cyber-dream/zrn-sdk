# CMake project for the IDE (CLion): libzrnui and the helloworld example, with
# real include paths, so the editor resolves everything. The Makefiles and
# `zrn build` stay what builds and ships the apps.
cmake_minimum_required(VERSION 3.16)
project(ztOS-libUI C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZUI_DEPS REQUIRED IMPORTED_TARGET
	wayland-client wayland-egl egl glesv2 freetype2 harfbuzz fontconfig libpng xkbcommon gbm)

file(GLOB ZUI_SOURCES CONFIGURE_DEPENDS libzrnui/src/*.c libzrnui/src/widgets/*.c)
add_library(zrnui SHARED ${ZUI_SOURCES} libzrnui/protocol/xdg-shell-protocol.c)
target_include_directories(zrnui
	PUBLIC libzrnui/include
	PRIVATE libzrnui/protocol libzrnui/src)
target_compile_options(zrnui PRIVATE -Wall -Wextra -Wno-unused-parameter
	-Wno-missing-field-initializers -fvisibility=hidden)
target_compile_definitions(zrnui PRIVATE ZUI_DATADIR="/usr/local/share/zrnui")
target_link_libraries(zrnui PRIVATE PkgConfig::ZUI_DEPS m pthread dl)
set_target_properties(zrnui PROPERTIES VERSION 0.1.0 SOVERSION 0)

# the examples: CLion runs them from .run/*.xml with ZUI_WINDOW=480x800 in the
# environment (the window, in points, on a Wayland session). On a shell:
#   ZUI_WINDOW=480x800 ZUI_STYLE=dotmatrix ./helloworld
foreach(example IN ITEMS helloworld-c hello-c settings-c)
	string(REGEX REPLACE "-c$" "" short ${example})
	add_executable(${short} examples/${example}/main.c)
	target_link_libraries(${short} PRIVATE zrnui)
endforeach()
