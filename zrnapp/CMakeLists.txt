cmake_minimum_required(VERSION 3.16)

project(zrnapp VERSION 2.0.0 DESCRIPTION "zrnapp - the app cycle and the OS integration of zrnui" LANGUAGES C)

# ON when zrnapp is the project being built, OFF when it is added with add_subdirectory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(ZRNAPP_IS_TOP_LEVEL ON)
else()
	set(ZRNAPP_IS_TOP_LEVEL OFF)
endif()

option(ZRNAPP_BUILD_SHARED "Build zrnapp as a shared library" OFF)
# the zrnui that is installed (zrnui.pc) instead of the one in ../zrnui; a package of zrnapp turns it ON
option(ZRNAPP_BUILD_TESTS "Build the tests" ${ZRNAPP_IS_TOP_LEVEL})
option(ZRNAPP_SYSTEM_ZRNUI "Link the installed zrnui (found with pkg-config) instead of building ../zrnui" OFF)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

# the UI core: the one that draws; zrnapp only talks to it through zrnui/platform.h
if(NOT TARGET zrnui::zrnui)
	if(ZRNAPP_SYSTEM_ZRNUI OR NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../zrnui/CMakeLists.txt)
		pkg_check_modules(ZRNUI REQUIRED IMPORTED_TARGET GLOBAL zrnui)
		add_library(zrnui::zrnui ALIAS PkgConfig::ZRNUI)
	else()
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../zrnui ${CMAKE_CURRENT_BINARY_DIR}/zrnui)
	endif()
endif()

pkg_check_modules(WAYLAND IMPORTED_TARGET wayland-client xkbcommon)
pkg_check_modules(GLES IMPORTED_TARGET egl glesv2)
pkg_check_modules(WAYLAND_EGL IMPORTED_TARGET wayland-egl)

# The protocols the window speaks are generated from their XML (the package wayland-protocols) with wayland-scanner at build
# time, not kept in the repository: xdg-shell (the window), viewporter and fractional-scale-v1 (the scale of the shell).
set(ZRNAPP_PROTOCOLS "")
if(WAYLAND_FOUND)
	pkg_get_variable(WAYLAND_PROTOCOLS_DIR wayland-protocols pkgdatadir)
	pkg_get_variable(WAYLAND_SCANNER wayland-scanner wayland_scanner)
	if(NOT WAYLAND_SCANNER)
		find_program(WAYLAND_SCANNER wayland-scanner)
	endif()
	set(ZRNAPP_PROTOCOL_XML
		xdg-shell|stable/xdg-shell/xdg-shell.xml
		viewporter|stable/viewporter/viewporter.xml
		fractional-scale-v1|staging/fractional-scale/fractional-scale-v1.xml)
	set(ZRNAPP_PROTOCOLS_OK ON)
	if(NOT WAYLAND_SCANNER OR NOT WAYLAND_PROTOCOLS_DIR)
		set(ZRNAPP_PROTOCOLS_OK OFF)
	else()
		foreach(entry ${ZRNAPP_PROTOCOL_XML})
			string(REPLACE "|" ";" parts ${entry})
			list(GET parts 1 relative)
			if(NOT EXISTS ${WAYLAND_PROTOCOLS_DIR}/${relative})
				set(ZRNAPP_PROTOCOLS_OK OFF)
			endif()
		endforeach()
	endif()
	if(NOT ZRNAPP_PROTOCOLS_OK)
		message(WARNING "wayland-scanner or the XML of wayland-protocols (xdg-shell, viewporter, fractional-scale-v1) not found: Zwindow_run will fail at run time")
		set(WAYLAND_FOUND OFF)
	endif()
endif()

set(ZRNAPP_PROTOCOL_DIR ${CMAKE_CURRENT_BINARY_DIR}/protocol)
if(WAYLAND_FOUND)
	file(MAKE_DIRECTORY ${ZRNAPP_PROTOCOL_DIR})
	foreach(entry ${ZRNAPP_PROTOCOL_XML})
		string(REPLACE "|" ";" parts ${entry})
		list(GET parts 0 name)
		list(GET parts 1 relative)
		set(xml ${WAYLAND_PROTOCOLS_DIR}/${relative})
		add_custom_command(
			OUTPUT ${ZRNAPP_PROTOCOL_DIR}/${name}-client-protocol.h ${ZRNAPP_PROTOCOL_DIR}/${name}-protocol.c
			COMMAND ${WAYLAND_SCANNER} client-header ${xml} ${ZRNAPP_PROTOCOL_DIR}/${name}-client-protocol.h
			COMMAND ${WAYLAND_SCANNER} private-code ${xml} ${ZRNAPP_PROTOCOL_DIR}/${name}-protocol.c
			DEPENDS ${xml}
			VERBATIM)
		list(APPEND ZRNAPP_PROTOCOLS ${ZRNAPP_PROTOCOL_DIR}/${name}-protocol.c)
	endforeach()
endif()

set(ZRNAPP_SOURCES src/window.c src/app_id.c ${ZRNAPP_PROTOCOLS})
if(WAYLAND_FOUND)
	# the GPU window needs EGL on the Wayland surface; without it the window draws with wl_shm
	if(GLES_FOUND AND WAYLAND_EGL_FOUND)
		list(APPEND ZRNAPP_SOURCES src/egl_window.c)
	endif()
endif()

if(ZRNAPP_BUILD_SHARED)
	add_library(zrnapp SHARED ${ZRNAPP_SOURCES})
	target_compile_definitions(zrnapp PRIVATE ZRNAPP_BUILD_DLL PUBLIC ZRNAPP_USE_DLL)
	set_target_properties(zrnapp PROPERTIES C_VISIBILITY_PRESET hidden)
else()
	add_library(zrnapp STATIC ${ZRNAPP_SOURCES})
endif()
add_library(zrnapp::zrnapp ALIAS zrnapp)

target_include_directories(zrnapp PUBLIC include PRIVATE src ${ZRNAPP_PROTOCOL_DIR})
target_link_libraries(zrnapp PUBLIC zrnui::zrnui)
target_compile_definitions(zrnapp PRIVATE _GNU_SOURCE)

if(WAYLAND_FOUND)
	target_link_libraries(zrnapp PRIVATE PkgConfig::WAYLAND)
	target_compile_definitions(zrnapp PRIVATE ZRNAPP_HAVE_WAYLAND)
	if(GLES_FOUND AND WAYLAND_EGL_FOUND)
		target_link_libraries(zrnapp PRIVATE PkgConfig::GLES PkgConfig::WAYLAND_EGL)
		target_compile_definitions(zrnapp PRIVATE ZRNAPP_HAVE_EGL)
	endif()
else()
	message(WARNING "wayland-client / xkbcommon not found: Zwindow_run will fail at run time")
endif()

# install: the library, its header and zrnapp.pc
include(GNUInstallDirs)
if(ZRNAPP_IS_TOP_LEVEL)
	install(TARGETS zrnapp ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(FILES include/zrnapp.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
	set(ZRNAPP_PC_REQUIRES_PRIVATE "wayland-client xkbcommon")
	if(GLES_FOUND AND WAYLAND_EGL_FOUND)
		string(APPEND ZRNAPP_PC_REQUIRES_PRIVATE " egl glesv2 wayland-egl")
	endif()
	configure_file(zrnapp.pc.in ${CMAKE_CURRENT_BINARY_DIR}/zrnapp.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/zrnapp.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(zrnapp PRIVATE -Wall -Wextra -Wno-unused-parameter)
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		target_compile_options(zrnapp PRIVATE -Wno-old-style-declaration)
	endif()
endif()

if(ZRNAPP_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
