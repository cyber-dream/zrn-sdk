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
	{ .name = "FOREGROUND", .dark = 0xFFFFFFFF, .light = 0xFF000000, .origin = "PhoneForegroundColor" },
	{ .name = "BACKGROUND", .dark = 0xFF000000, .light = 0xFFFFFFFF, .origin = "PhoneBackgroundColor" },
	{ .name = "CONTRAST_FOREGROUND", .dark = 0xFF000000, .light = 0xFFFFFFFF, .origin = "PhoneContrastForegroundColor" },
	{ .name = "CONTRAST_BACKGROUND", .dark = 0xFFFFFFFF, .light = 0xDE000000, .origin = "PhoneContrastBackgroundColor" },
	{ .name = "DISABLED", .dark = 0x66FFFFFF, .light = 0x4D000000, .origin = "PhoneDisabledColor" },
	{ .name = "PROGRESS_BAR_BACKGROUND", .dark = 0x19FFFFFF, .light = 0x19000000, .origin = "ProgressBarBackgroundBrush" },
	{ .name = "TEXT_BOX", .dark = 0xBFFFFFFF, .light = 0x26000000, .origin = "PhoneTextBoxBrush" },
	{ .name = "TEXT_BOX_FOREGROUND", .dark = 0xFF000000, .light = 0xDE000000, .origin = "PhoneTextBoxForegroundColorBrush" },
	{ .name = "TEXT_BOX_EDIT_BACKGROUND", .dark = 0xFFFFFFFF, .light = 0x00000000, .origin = "PhoneTextBoxEditBackgroundBrush" },
	{ .name = "TEXT_BOX_READ_ONLY", .dark = 0x77000000, .light = 0x2E000000, .origin = "PhoneTextBoxReadOnlyBrush" },
	{ .name = "TEXT_CARET", .dark = 0xFF000000, .light = 0xDE000000, .origin = "PhoneCaretBrush" },
	{ .name = "SUBTLE", .dark = 0x99FFFFFF, .light = 0x66000000, .origin = "PhoneSubtleColor" },
	{ .name = "BUTTON_PRESSED_FOREGROUND", .dark = 0xFFFFFFFF, .light = 0xFFFFFFFF, .origin = "ButtonPressedForegroundThemeBrush (8.1)" },
	{ .name = "CHROME", .dark = 0xFF1F1F1F, .light = 0xFFDDDDDD, .origin = "PhoneChromeBrush" },
	{ .name = "INACTIVE", .dark = 0x33FFFFFF, .light = 0x33000000, .origin = "PhoneInactiveColor" },
	{ .name = "BORDER", .dark = 0xBFFFFFFF, .light = 0x99000000, .origin = "PhoneBorderBrush" },
	{ .name = "SEMITRANSPARENT", .dark = 0xAA000000, .light = 0xAAFFFFFF, .origin = "PhoneSemitransparentBrush" },
	{ .name = "ACCENT", .dark = 0, .light = 0, .origin = "the accent of the root, not of the theme" },
	{ .name = "BASE_MEDIUM_HIGH", .dark = 0xCCFFFFFF, .light = 0xBF000000, .origin = "PhoneBaseMediumHighColor (8.1)" },
	{ .name = "BASE_MID", .dark = 0x99FFFFFF, .light = 0x73000000, .origin = "PhoneBaseMidColor (8.1)" },
	{ .name = "BASE_MEDIUM_MID", .dark = 0x80FFFFFF, .light = 0x80000000, .origin = "PhoneBaseMediumMidColor (8.1)" },
	{ .name = "BASE_MEDIUM_MID_SOLID", .dark = 0xFF666666, .light = 0xFFCCCCCC, .origin = "PhoneBaseMediumMidSolidColor (8.1)" },
	{ .name = "BASE_LOW", .dark = 0x73FFFFFF, .light = 0x40000000, .origin = "PhoneBaseLowColor (8.1)" },
	{ .name = "BASE_LOW_SOLID", .dark = 0xFF747474, .light = 0xFFBFBFBF, .origin = "PhoneBaseLowSolidColor (8.1)" },
	{ .name = "TEXT_OVER_ACCENT", .dark = 0xFFFFFFFF, .light = 0xFFFFFFFF, .origin = "the text drawn over the accent" },
	{ .name = "TEXT_BOX_BACKGROUND_81", .dark = 0xFFFFFFFF, .light = 0xFFBFBFBF, .origin = "TextBoxBackgroundThemeBrush (8.1)" },
	{ .name = "TEXT_BOX_BORDER_81", .dark = 0xFFFFFFFF, .light = 0xFFBFBFBF, .origin = "TextBoxBorderThemeBrush (8.1)" },
	{ .name = "TEXT_BOX_FOREGROUND_81", .dark = 0xFF000000, .light = 0xFF000000, .origin = "TextBoxForegroundThemeBrush (8.1)" },
	{ .name = "TEXT_BOX_PLACEHOLDER_81", .dark = 0xFF909090, .light = 0x40000000, .origin = "TextControlPlaceholderThemeBrush (8.1)" },
	{ .name = "TEXT_BOX_FOCUSED_BACKGROUND_81", .dark = 0xFFFFFFFF, .light = 0xFFFFFFFF, .origin = "TextBoxFocusedBackgroundThemeBrush (8.1)" },
	{ .name = "SLIDER_THUMB", .dark = 0xFFFFFFFF, .light = 0xFF000000, .origin = "SliderThumbBackgroundThemeBrush (8.1)" },
	{ .name = "SLIDER_THUMB_DISABLED", .dark = 0xFF999999, .light = 0xFF808080, .origin = "SliderThumbDisabledBackgroundThemeBrush (8.1)" },
	{ .name = "SLIDER_TRACK", .dark = 0xFF1F1F1F, .light = 0xFFDEDEDE, .origin = "SliderTrackBackgroundThemeBrush (8.1)" },
	{ .name = "SLIDER_DECREASE_DISABLED", .dark = 0xFF666666, .light = 0xFFBDBDBD, .origin = "SliderBarDecreaseDisabledBackgroundThemeBrush (8.1)" },
	{ .name = "BASE_BLACK", .dark = 0xFF000000, .light = 0xFF000000, .origin = "PhoneBaseBlackColor (8.1)" },
	{ .name = "BASE_WHITE", .dark = 0xFFFFFFFF, .light = 0xFFFFFFFF, .origin = "PhoneBaseWhiteColor (8.1)" },
	{ .name = "PHONE_DISABLED", .dark = 0x66FFFFFF, .light = 0x66000000, .origin = "PhoneDisabledColor (8.1)" },
	{ .name = "TEXT_MID", .dark = 0x99FFFFFF, .light = 0x73000000, .origin = "PhoneMidColor" },
	{ .name = "TEXT_LOW", .dark = 0x73FFFFFF, .light = 0x40000000, .origin = "PhoneLowColor" },
	{ .name = "CONTENT_DIALOG_DIMMING", .dark = 0x99000000, .light = 0x99FFFFFF, .origin = "ContentDialogDimmingThemeBrush (8.1)" },
	{ .name = "LIST_BOX_FOREGROUND", .dark = 0xFF000000, .light = 0xFFFFFFFF, .origin = "ListBoxForeground (8.1, see the ListBox note in zrnui_controls.h)" }
};

static const char* const g_accent_names[] = { "Lime", "Green", "Emerald", "Teal", "Cyan", "Cobalt", "Indigo",
	"Violet", "Pink", "Magenta", "Crimson", "Red", "Orange", "Amber", "Yellow", "Brown", "Olive", "Steel",
	"Mauve", "Taupe" };

static constexpr uint32_t g_accent_values[] = {
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
	if (g_tables_ready) return;
	for (int i = 0; i < ZST_TC_COUNT; i++) {
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
	tables_init();
	for (int i = 0; i < ZST_TC_COUNT; i++) t->colors[i] = Zcolor_argb(source[i]);
	t->theme = theme;
	t->accent = accent;
	t->colors[ZST_TC_ACCENT] = accent;
	t->background = t->colors[ZST_TC_BACKGROUND];
}
