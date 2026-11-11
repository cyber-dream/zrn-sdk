/**
 * The configuration of the library: Zinit_ex takes it explicitly and never reads the environment; Zinit is the one
 * that does, through Zconfig_from_environment.
 */

#define _POSIX_C_SOURCE 200809L

#include "test.h"

static void test_defaults(void) {
	ZConfig config;
	Zconfig_default(&config);
	CHECK(config.font_directory == nullptr);
	CHECK(config.theme_file == nullptr);
	CHECK(config.renderer == Z_RENDERER_AUTO);
	CHECK(config.animations == Z_ANIMATIONS_AUTO);
	CHECK_NEAR(config.animation_speed, 1);
}

static void test_environment_is_read_by_the_helper(void) {
	setenv("ZRNUI_RENDERER", "software", 1);
	setenv("ZRNUI_ANIMATIONS", "off", 1);
	setenv("ZRNUI_ANIMATION_SPEED", "2.5", 1);
	setenv("ZRNUI_THEME", "/nowhere/theme.ini", 1);
	ZConfig config;
	Zconfig_from_environment(&config);
	CHECK(config.renderer == Z_RENDERER_SOFTWARE);
	CHECK(config.animations == Z_ANIMATIONS_OFF);
	CHECK(config.animation_speed > 2.4f && config.animation_speed < 2.6f);
	CHECK_STR(config.theme_file, "/nowhere/theme.ini");
	setenv("ZRNUI_ANIMATION_SPEED", "-3", 1);
	Zconfig_from_environment(&config);
	CHECK_NEAR(config.animation_speed, 1);   /* a speed that is not above 0 is the default */
}

static void test_explicit_config_ignores_the_environment(void) {
	setenv("ZRNUI_RENDERER", "gpu", 1);
	setenv("ZRNUI_ANIMATIONS", "on", 1);
	ZConfig config;
	Zconfig_default(&config);
	config.renderer = Z_RENDERER_SOFTWARE;
	config.animations = Z_ANIMATIONS_OFF;
	config.theme_file = "";
	CHECK(Zinit_ex(&config));
	const Zid root = Zroot_create();
	CHECK(Zroot_get_renderer(root) == Z_RENDERER_SOFTWARE);
	CHECK(Zroot_get_animations(root) == Z_ANIMATIONS_OFF);
	Zroot_destroy(root);
	Zshutdown();
}

static void test_zinit_reads_the_environment(void) {
	setenv("ZRNUI_RENDERER", "gpu", 1);
	setenv("ZRNUI_ANIMATIONS", "on", 1);
	CHECK(Zinit());
	const Zid root = Zroot_create();
	CHECK(Zroot_get_renderer(root) == Z_RENDERER_GPU);
	CHECK(Zroot_get_animations(root) == Z_ANIMATIONS_ON);
	Zroot_destroy(root);
	Zshutdown();
}

static void test_a_new_configuration_after_shutdown(void) {
	ZConfig config;
	Zconfig_default(&config);
	config.renderer = Z_RENDERER_GPU;
	CHECK(Zinit_ex(&config));
	Zshutdown();
	config.renderer = Z_RENDERER_SOFTWARE;
	CHECK(Zinit_ex(&config));
	const Zid root = Zroot_create();
	CHECK(Zroot_get_renderer(root) == Z_RENDERER_SOFTWARE);
	Zroot_destroy(root);
	Zshutdown();
}

int main(void) {
	RUN(test_defaults);
	RUN(test_environment_is_read_by_the_helper);
	RUN(test_explicit_config_ignores_the_environment);
	RUN(test_zinit_reads_the_environment);
	RUN(test_a_new_configuration_after_shutdown);
	return test_finish("test_config");
}
