# the app id of a window needs no compositor
add_executable(test_app_id test_app_id.c ../src/app_id.c)
target_include_directories(test_app_id PRIVATE ../src)
target_compile_definitions(test_app_id PRIVATE _GNU_SOURCE)
add_test(NAME test_app_id COMMAND test_app_id)

# the window on a real compositor: cage with a headless backend (no screen, no GPU needed for the software window)
find_program(ZRNAPP_CAGE cage)
if(ZRNAPP_CAGE AND WAYLAND_FOUND)
	add_executable(test_window test_window.c)
	target_link_libraries(test_window PRIVATE zrnapp)

	set(ZRNAPP_CAGE_ENV "WLR_BACKENDS=headless" "WLR_LIBINPUT_NO_DEVICES=1")
	add_test(NAME window_software COMMAND ${ZRNAPP_CAGE} -- $<TARGET_FILE:test_window>)
	set_tests_properties(window_software PROPERTIES
		ENVIRONMENT "${ZRNAPP_CAGE_ENV};WLR_RENDERER=pixman;ZRNUI_RENDERER=software;ZRNAPP_TEST_EXPECT=software"
		PASS_REGULAR_EXPRESSION "test_window: [0-9]+ checks, 0 failed"
		TIMEOUT 30)
	if(GLES_FOUND AND WAYLAND_EGL_FOUND)
		add_test(NAME window_gpu COMMAND ${ZRNAPP_CAGE} -- $<TARGET_FILE:test_window>)
		set_tests_properties(window_gpu PROPERTIES
			ENVIRONMENT "${ZRNAPP_CAGE_ENV};WLR_RENDERER=gles2;ZRNUI_RENDERER=gpu;ZRNAPP_TEST_EXPECT=gles2;ASAN_OPTIONS=detect_leaks=0"
			PASS_REGULAR_EXPRESSION "test_window: [0-9]+ checks, 0 failed"
			TIMEOUT 30)
	endif()
endif()
