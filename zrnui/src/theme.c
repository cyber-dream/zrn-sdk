/**
 * zrnui - theme.c
 * The WP8 theme constants (docs/wp8-reference/themes.md), Dark and Light, and the
 * Windows Phone 8.1 ones the XAML controls use (themeresources.xaml, the Default and Light dictionaries).
 *
 * The readable source of the values is theme.ini next to the sources: the
 * sections [dark] / [light] / [accent], one "NAME = #AARRGGBB" per color. The
 * built-in table below holds the same values and serves as the default; every
 * key of the file replaces it. The file is looked up in $ZRNUI_THEME, then
 * the path compiled in (ZRNUI_THEME_FILE). Missing keys and a missing file
 * change nothing.
 */

#define _POSIX_C_SOURCE 200809L
#include "internal.h"

#include <stdio.h>
#include <unistd.h>

typedef struct {
	const char* name;   /* the key of theme.ini, without the ZST_TC_ prefix */
	uint32_t dark;
	uint32_t light;
	const char* origin; /* the brush or color of the WP8 / 8.1 themes the value comes from */
} zrn_theme_color_t;

static const zrn_theme_color_t g_theme_colors[ZST_TC_COUNT] = {
	{ "FOREGROUND", 0xFFFFFFFF, 0xFF000000, "PhoneForegroundColor" },
	{ "BACKGROUND", 0xFF000000, 0xFFFFFFFF, "PhoneBackgroundColor" },
	{ "CONTRAST_FOREGROUND", 0xFF000000, 0xFFFFFFFF, "PhoneContrastForegroundColor" },
	{ "CONTRAST_BACKGROUND", 0xFFFFFFFF, 0xDE000000, "PhoneContrastBackgroundColor" },
	{ "DISABLED", 0x66FFFFFF, 0x4D000000, "PhoneDisabledColor" },
	{ "PROGRESS_BAR_BACKGROUND", 0x19FFFFFF, 0x19000000, "ProgressBarBackgroundBrush" },
	{ "TEXT_BOX", 0xBFFFFFFF, 0x26000000, "PhoneTextBoxBrush" },
	{ "TEXT_BOX_FOREGROUND", 0xFF000000, 0xDE000000, "PhoneTextBoxForegroundColorBrush" },
	{ "TEXT_BOX_EDIT_BACKGROUND", 0xFFFFFFFF, 0x00000000, "PhoneTextBoxEditBackgroundBrush" },
	{ "TEXT_BOX_READ_ONLY", 0x77000000, 0x2E000000, "PhoneTextBoxReadOnlyBrush" },
	{ "TEXT_CARET", 0xFF000000, 0xDE000000, "PhoneCaretBrush" },
	{ "SUBTLE", 0x99FFFFFF, 0x66000000, "PhoneSubtleColor" },
	{ "BUTTON_PRESSED_FOREGROUND", 0xFFFFFFFF, 0xFFFFFFFF, "ButtonPressedForegroundThemeBrush (8.1)" },
	{ "CHROME", 0xFF1F1F1F, 0xFFDDDDDD, "PhoneChromeBrush" },
	{ "INACTIVE", 0x33FFFFFF, 0x33000000, "PhoneInactiveColor" },
	{ "BORDER", 0xBFFFFFFF, 0x99000000, "PhoneBorderBrush" },
	{ "SEMITRANSPARENT", 0xAA000000, 0xAAFFFFFF, "PhoneSemitransparentBrush" },
	{ "ACCENT", 0, 0, "the accent of the root, not of the theme" },
	{ "BASE_MEDIUM_HIGH", 0xCCFFFFFF, 0xBF000000, "PhoneBaseMediumHighColor (8.1)" },
	{ "BASE_MID", 0x99FFFFFF, 0x73000000, "PhoneBaseMidColor (8.1)" },
	{ "BASE_MEDIUM_MID", 0x80FFFFFF, 0x80000000, "PhoneBaseMediumMidColor (8.1)" },
	{ "BASE_MEDIUM_MID_SOLID", 0xFF666666, 0xFFCCCCCC, "PhoneBaseMediumMidSolidColor (8.1)" },
	{ "BASE_LOW", 0x73FFFFFF, 0x40000000, "PhoneBaseLowColor (8.1)" },
	{ "BASE_LOW_SOLID", 0xFF747474, 0xFFBFBFBF, "PhoneBaseLowSolidColor (8.1)" },
	{ "TEXT_OVER_ACCENT", 0xFFFFFFFF, 0xFFFFFFFF, "the text drawn over the accent" },
	{ "TEXT_BOX_BACKGROUND_81", 0xFFFFFFFF, 0xFFBFBFBF, "TextBoxBackgroundThemeBrush (8.1)" },
	{ "TEXT_BOX_BORDER_81", 0xFFFFFFFF, 0xFFBFBFBF, "TextBoxBorderThemeBrush (8.1)" },
	{ "TEXT_BOX_FOREGROUND_81", 0xFF000000, 0xFF000000, "TextBoxForegroundThemeBrush (8.1)" },
	{ "TEXT_BOX_PLACEHOLDER_81", 0xFF909090, 0x40000000, "TextControlPlaceholderThemeBrush (8.1)" },
	{ "TEXT_BOX_FOCUSED_BACKGROUND_81", 0xFFFFFFFF, 0xFFFFFFFF, "TextBoxFocusedBackgroundThemeBrush (8.1)" },
	{ "SLIDER_THUMB", 0xFFFFFFFF, 0xFF000000, "SliderThumbBackgroundThemeBrush (8.1)" },
	{ "SLIDER_THUMB_DISABLED", 0xFF999999, 0xFF808080, "SliderThumbDisabledBackgroundThemeBrush (8.1)" },
	{ "SLIDER_TRACK", 0xFF1F1F1F, 0xFFDEDEDE, "SliderTrackBackgroundThemeBrush (8.1)" },
	{ "SLIDER_DECREASE_DISABLED", 0xFF666666, 0xFFBDBDBD, "SliderBarDecreaseDisabledBackgroundThemeBrush (8.1)" },
	{ "BASE_BLACK", 0xFF000000, 0xFF000000, "PhoneBaseBlackColor (8.1)" },
	{ "BASE_WHITE", 0xFFFFFFFF, 0xFFFFFFFF, "PhoneBaseWhiteColor (8.1)" },
	{ "PHONE_DISABLED", 0x66FFFFFF, 0x66000000, "PhoneDisabledColor (8.1)" },
	{ "TEXT_MID", 0x99FFFFFF, 0x73000000, "PhoneMidColor" },
	{ "TEXT_LOW", 0x73FFFFFF, 0x40000000, "PhoneLowColor" },
	{ "CONTENT_DIALOG_DIMMING", 0x99000000, 0x99FFFFFF, "ContentDialogDimmingThemeBrush (8.1)" },
	{ "LIST_BOX_FOREGROUND", 0xFF000000, 0xFFFFFFFF, "ListBoxForeground (8.1, see the ListBox note in zrnui_controls.h)" }
};

static const char* const g_accent_names[] = { "Lime", "Green", "Emerald", "Teal", "Cyan", "Cobalt", "Indigo",
	"Violet", "Pink", "Magenta", "Crimson", "Red", "Orange", "Amber", "Yellow", "Brown", "Olive", "Steel",
	"Mauve", "Taupe" };

static const uint32_t g_accent_values[] = {
	0xFFA4C400, /* Lime */    0xFF60A917, /* Green */   0xFF008A00, /* Emerald */ 0xFF00ABA9, /* Teal */
	0xFF1BA1E2, /* Cyan */    0xFF0050EF, /* Cobalt */  0xFF6A00FF, /* Indigo */  0xFFAA00FF, /* Violet */
	0xFFF472D0, /* Pink */    0xFFD80073, /* Magenta */ 0xFFA20025, /* Crimson */ 0xFFE51400, /* Red */
	0xFFFA6800, /* Orange */  0xFFF0A30A, /* Amber */   0xFFE3C800, /* Yellow */  0xFF825A2C, /* Brown */
	0xFF6D8764, /* Olive */   0xFF647687, /* Steel */   0xFF76608A, /* Mauve */   0xFF87794E  /* Taupe */
};

/* The resolved tables: the built-in values with theme.ini over them. */
static uint32_t g_dark[ZST_TC_COUNT];
static uint32_t g_light[ZST_TC_COUNT];
static uint32_t g_accents[sizeof g_accent_values / sizeof g_accent_values[0]];
static int g_tables_ready;

/** Reads #RRGGBB or #AARRGGBB. */
static int ini_color(const char* s, uint32_t* out) {
	uint32_t v = 0;
	int digits = 0;
	if (*s == '#') s++;
	while (s[digits]) {
		const char c = s[digits];
		const int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
		if (d < 0) return 0;
		v = v * 16 + (uint32_t)d;
		digits++;
	}
	if (digits == 6) v |= 0xFF000000;
	else if (digits != 8) return 0;
	*out = v;
	return 1;
}

static void ini_assign(const char* const section, const char* const key, const uint32_t v) {
	size_t i;
	if (strcmp(section, "accent") == 0) {
		for (i = 0; i < sizeof g_accents / sizeof g_accents[0]; i++)
			if (strcmp(key, g_accent_names[i]) == 0) g_accents[i] = v;
		return;
	}
	for (i = 0; i < ZST_TC_COUNT; i++) {
		if (strcmp(g_theme_colors[i].name, key) != 0) continue;
		if (i == ZST_TC_ACCENT) return; /* the accent comes from the root, not from a theme section */
		if (strcmp(section, "dark") == 0) g_dark[i] = v;
		else if (strcmp(section, "light") == 0) g_light[i] = v;
		return;
	}
}

/** Applies theme.ini over the built-in tables: [dark] / [light] / [accent], "NAME = #AARRGGBB". */
static void ini_apply(void) {
	const char* path = getenv("ZRNUI_THEME");
	char beside[512], *slash;
	FILE* file;
	char line[512];
	char section[sizeof line] = "";
	if (!path || !*path) {
		/* beside the binary first (a phone has no build tree), then the compiled path */
		const ssize_t n = readlink("/proc/self/exe", beside, sizeof beside - 1);
		if (n > 0) {
			beside[n] = 0;
			slash = strrchr(beside, '/');
			if (slash) {
				snprintf(slash + 1, sizeof beside - (size_t)(slash + 1 - beside), "theme.ini");
				file = fopen(beside, "r");
				if (file) goto read;
			}
		}
		path = ZRNUI_THEME_FILE;
	}
	if (!path || !*path) return;
	file = fopen(path, "r");
	if (!file) {
		zrn_error(Z_ERROR_IO, "theme file not opened: %s", path);
		return;
	}
read:
	while (fgets(line, sizeof line, file)) {
		char *p = line, *eq, *end, *value;
		uint32_t v;
		while (*p == ' ' || *p == '\t') p++;
		if (*p == ';' || *p == '#' || *p == 0 || *p == '\n' || *p == '\r') continue; /* a comment or an empty line */
		if (*p == '[') {
			char* const close = strchr(p, ']');
			if (!close) continue;
			if ((size_t)(close - p - 1) < sizeof section - 1) {
				memcpy(section, p + 1, (size_t)(close - p - 1));
				section[close - p - 1] = 0;
			} else section[0] = 0;
			continue;
		}
		eq = strchr(p, '=');
		if (!eq || !*section) continue;
		*eq = 0;
		value = eq + 1;
		/* the value runs to an inline comment or the end of the line */
		end = strchr(value, ';');
		if (end) *end = 0;
		end = value + strlen(value);
		while (end > value && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\n' || end[-1] == '\r')) end--;
		*end = 0;
		/* the key: cut the trailing spaces */
		end = eq;
		while (end > p && (end[-1] == ' ' || end[-1] == '\t')) end--;
		*end = 0;
		while (*value == ' ' || *value == '\t') value++;
		if (ini_color(value, &v)) ini_assign(section, p, v);
	}
	fclose(file);
}

static void tables_init(void) {
	int i;
	if (g_tables_ready) return;
	for (i = 0; i < ZST_TC_COUNT; i++) {
		g_dark[i] = g_theme_colors[i].dark;
		g_light[i] = g_theme_colors[i].light;
	}
	memcpy(g_accents, g_accent_values, sizeof g_accents);
	ini_apply();
	g_tables_ready = 1;
}

/** Forgets the tables read from theme.ini: the next use reads the file again (the fuzzers change it between runs). */
void zrn_theme_reset(void) { g_tables_ready = 0; }

ZColor zrn_accent_color(const ZAccent accent) {
	unsigned index = (unsigned)accent - (unsigned)Z_ACCENT_LIME;
	if (index >= sizeof g_accents / sizeof g_accents[0]) index = 5; /* Cobalt */
	tables_init();
	return Zcolor_argb(g_accents[index]);
}

void zrn_theme_init(zrn_theme_t* const t, const ZTheme theme, const ZColor accent) {
	const uint32_t* source = theme == Z_THEME_LIGHT ? g_light : g_dark;
	int i;
	tables_init();
	for (i = 0; i < ZST_TC_COUNT; i++) t->colors[i] = Zcolor_argb(source[i]);
	t->theme = theme;
	t->accent = accent;
	t->colors[ZST_TC_ACCENT] = accent;
	t->background = t->colors[ZST_TC_BACKGROUND];
}
