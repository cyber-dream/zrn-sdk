#!/usr/bin/env python3
"""
Kestrel - tools/gen_theme_resources.py

Generates the ThemeResource tables of Windows Phone 8.1 from themeresources.xaml:

    include/kestrel/kestrel_resources.h   enum KThemeResource, one K_RES_<SNAKE_CASE_OF_KEY> per unique key
    src/theme_resources.c                 per key: type, the Dark ("Default") and the Light value, the accent flags

The generated files are CHECKED IN: the build does not need python. To regenerate, from kestrel/:

    python3 tools/gen_theme_resources.py [path/to/themeresources.xaml]

(the default path is /home/dart/Documents/metro/xaml-docs/themeresources.xaml). Python 3 standard library only.

What is read: the ThemeDictionaries "Default" (Dark) and "Light" (HighContrast* are ignored) and the resources
outside them (x:Double, x:Int32, Thickness, FontFamily, FontWeight, Color, SolidColorBrush), the same in both
themes. Styles, DataTemplates and ControlTemplates are ignored. A brush is resolved to an ARGB color:
Color="#AARRGGBB", Color="Transparent" or Color="{ThemeResource X}" / "{StaticResource X}" (a Color or a brush of
the same dictionary, then of the top level), Opacity scales the alpha. SystemColorControlAccentColor is the accent
of the system: a color that is an accent (or is made of it) is flagged and follows the accent of the root, only its
alpha is kept in the table. A key that cannot be resolved is skipped and listed in the comment of the generated files.
"""

import os
import re
import sys
import xml.etree.ElementTree as ET

NS_X = '{http://schemas.microsoft.com/winfx/2006/xaml}'
ACCENT_KEY = 'SystemColorControlAccentColor'
ENUM_FIRST = 0x3000
ENUM_USER_BASE = 0x8000
DEFAULT_SOURCE = '/home/dart/Documents/metro/xaml-docs/themeresources.xaml'

FONT_FAMILIES = {
    'Segoe WP': 'K_FONT_NORMAL',
    'Segoe WP Light': 'K_FONT_LIGHT',
    'Segoe WP SemiLight': 'K_FONT_SEMILIGHT',
    'Segoe WP Semibold': 'K_FONT_SEMIBOLD',
    'Segoe WP Bold': 'K_FONT_BOLD',
}
# FontWeight is an int: the weight of the OpenType scale
FONT_WEIGHTS = {'Thin': 100, 'ExtraLight': 200, 'Light': 300, 'SemiLight': 350, 'Normal': 400, 'Regular': 400,
                'Medium': 500, 'SemiBold': 600, 'Semibold': 600, 'Bold': 700, 'ExtraBold': 800, 'Black': 900,
                'ExtraBlack': 950}

COLOR, DOUBLE, INT, THICKNESS, FONT_FAMILY = 'COLOR', 'DOUBLE', 'INT', 'THICKNESS', 'FONT_FAMILY'


def local(tag):
    return tag.split('}')[1]


def snake(key):
    s = re.sub(r'(?<=[a-z0-9])(?=[A-Z])', '_', key)
    s = re.sub(r'(?<=[A-Z])(?=[A-Z][a-z])', '_', s)
    return 'K_RES_' + s.upper()


def parse_argb(text):
    text = text.strip()
    if text == 'Transparent':
        return 0x00FFFFFF
    m = re.fullmatch(r'#([0-9A-Fa-f]{8})', text)
    if m:
        return int(m.group(1), 16)
    m = re.fullmatch(r'#([0-9A-Fa-f]{6})', text)
    if m:
        return 0xFF000000 | int(m.group(1), 16)
    return None


def float_text(f):
    s = repr(float(f))
    if '.' not in s and 'e' not in s:
        s += '.0'
    return s + 'f'


def parse_thickness(text):
    p = [float(v) for v in re.split(r'[,\s]+', text.strip())]
    if len(p) == 1:
        return (p[0],) * 4
    if len(p) == 2:  # horizontal, vertical
        return (p[0], p[1], p[0], p[1])
    if len(p) == 4:
        return tuple(p)
    raise ValueError(text)


class Dict:
    """The elements of one ResourceDictionary, by key, in file order."""

    def __init__(self, node):
        self.items = {}
        for el in node:
            tag = local(el.tag)
            key = el.get(NS_X + 'Key')
            if key is None or tag not in ('Color', 'SolidColorBrush', 'Double', 'Int32', 'Thickness', 'FontFamily', 'FontWeight'):
                continue
            self.items[key] = el


def resolve_color(key, theme, top, stack=()):
    """(argb, accent) of a Color or a SolidColorBrush key, or None when it cannot be resolved."""
    if key == ACCENT_KEY:
        return (0xFF000000, True)
    if key in stack:
        return None
    el = theme.items.get(key)
    if el is None:
        el = top.items.get(key)
    if el is None:
        return None
    tag = local(el.tag)
    if tag == 'Color':
        value = (el.text or '').strip()
    elif tag == 'SolidColorBrush':
        value = el.get('Color')
    else:
        return None
    if value is None:
        return None
    m = re.fullmatch(r'\{(?:ThemeResource|StaticResource) ([A-Za-z0-9_]+)\}', value.strip())
    if m:
        r = resolve_color(m.group(1), theme, top, stack + (key,))
    else:
        argb = parse_argb(value)
        r = None if argb is None else (argb, False)
    if r is None:
        return None
    argb, accent = r
    opacity = el.get('Opacity')
    if opacity is not None:
        a = int(round(((argb >> 24) & 0xFF) * float(opacity)))
        argb = (a << 24) | (argb & 0xFFFFFF)
    if accent:
        argb &= 0xFF000000
    return (argb, accent)


def resolve(el, theme, top):
    """(type, value, accent) of one element, or None. value: argb int, float, int, 4 floats, enum name."""
    tag = local(el.tag)
    key = el.get(NS_X + 'Key')
    text = (el.text or '').strip()
    if tag in ('Color', 'SolidColorBrush'):
        r = resolve_color(key, theme, top)
        return None if r is None else (COLOR, r[0], r[1])
    if tag == 'Double':
        return (DOUBLE, float(text), False)
    if tag == 'Int32':
        return (INT, int(text), False)
    if tag == 'Thickness':
        return (THICKNESS, parse_thickness(text), False)
    if tag == 'FontFamily':
        return (FONT_FAMILY, FONT_FAMILIES[text], False) if text in FONT_FAMILIES else None
    if tag == 'FontWeight':
        return (INT, FONT_WEIGHTS[text], False) if text in FONT_WEIGHTS else None
    return None


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    root_dir = os.path.dirname(here)
    source = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_SOURCE
    tree = ET.parse(source)
    document = tree.getroot()

    theme_dicts = {}
    for d in document.iter():
        if local(d.tag) == 'ResourceDictionary' and d.get(NS_X + 'Key') in ('Default', 'Light'):
            theme_dicts[d.get(NS_X + 'Key')] = Dict(d)
    top = Dict(document)  # the direct children of the document only
    dark, light = theme_dicts['Default'], theme_dicts['Light']

    # the unique keys: those of the theme dictionaries (in the order of Default), then the top level ones
    keys = []
    for k in dark.items:
        keys.append((k, 'theme'))
    for k in light.items:
        if k not in dark.items:
            raise SystemExit('Light has a key Default has not: ' + k)
    for k in top.items:
        if k in dark.items:
            raise SystemExit('a key of the top level and of a theme dictionary: ' + k)
        keys.append((k, 'top'))
    keys.append((ACCENT_KEY, 'synthetic'))

    entries = []  # (key, type, dark, light, accent_dark, accent_light)
    skipped = []
    for key, where in keys:
        if where == 'synthetic':
            entries.append((key, COLOR, 0xFF000000, 0xFF000000, True, True))
            continue
        if where == 'theme':
            ed, el_ = dark.items[key], light.items[key]
            rd, rl = resolve(ed, dark, top), resolve(el_, light, top)
        else:
            ed = top.items[key]
            rd = rl = resolve(ed, Dict(ET.Element('x')), top)
        if rd is None or rl is None:
            skipped.append(key)
            continue
        if rd[0] != rl[0]:
            raise SystemExit('type differs between themes: ' + key)
        entries.append((key, rd[0], rd[1], rl[1], rd[2], rl[2]))

    names = {}
    for e in entries:
        n = snake(e[0])
        if n in names:
            raise SystemExit('name clash: %s and %s' % (names[n], e[0]))
        names[n] = e[0]

    counts = {}
    for e in entries:
        counts[e[1]] = counts.get(e[1], 0) + 1
    summary = ', '.join('%d %s' % (counts[t], t.lower()) for t in (COLOR, DOUBLE, INT, THICKNESS, FONT_FAMILY) if t in counts)
    skipped_text = ', '.join(skipped) if skipped else 'none'
    banner = ('/* GENERATED by tools/gen_theme_resources.py from themeresources.xaml: do not edit.\n'
              ' * Regenerate (from kestrel/): python3 tools/gen_theme_resources.py [themeresources.xaml]\n'
              ' * %d keys: %s.\n'
              ' * Skipped keys that could not be resolved: %s.\n'
              ' * K_RES_SYSTEM_COLOR_CONTROL_ACCENT_COLOR is not in the XAML file: it is the accent of the system that the brushes\n'
              ' * of the file refer to ({ThemeResource SystemColorControlAccentColor}), the accent of the root. */\n'
              % (len(entries), summary, skipped_text))

    # ---- the header
    h = ['/**\n * Kestrel\n * kestrel_resources.h : the keys of the ThemeResource of Windows Phone 8.1 (themeresources.xaml).\n *\n'
         ' * Read a value of the theme of a root with Kroot_get_theme_color / double / int / thickness / font_family,\n'
         ' * override it with Kroot_set_resource_*, bind a property to a key with Kcontrol_bind_* (kestrel_host.h).\n'
         ' * An application has keys of its own at K_RES_USER_BASE and above.\n */\n\n',
         banner, '\n#ifndef KESTREL_RESOURCES_H\n#define KESTREL_RESOURCES_H\n\n',
         'typedef enum KThemeResource {\n\tK_RES_NONE = 0,\n']
    h.append('\tK_RES_FIRST_ = 0x%04X,\n' % ENUM_FIRST)
    for i, e in enumerate(entries):
        n = snake(e[0])
        h.append('\t%s%s,\n' % (n, ' = K_RES_FIRST_' if i == 0 else ''))
    h.append('\tK_RES_LAST_,\n\t/** The first key an application can define for itself: any value from here is a user key. */\n'
             '\tK_RES_USER_BASE = 0x%04X\n} KThemeResource;\n\n#endif\n' % ENUM_USER_BASE)
    with open(os.path.join(root_dir, 'include', 'kestrel', 'kestrel_resources.h'), 'w', newline='\n') as f:
        f.write(''.join(h))

    # ---- the tables
    def val(t, v):
        if t == COLOR:
            return '.argb = 0x%08XU' % v
        if t == DOUBLE:
            return '.f = { %s }' % float_text(v)
        if t == INT:
            return '.i = %d' % v
        if t == THICKNESS:
            return '.f = { %s }' % ', '.join(float_text(x) for x in v)
        return '.i = %s' % v

    c = ['/**\n * Kestrel - theme_resources.c\n * The ThemeResource values of Windows Phone 8.1, Dark and Light: data only, see resources.c.\n */\n\n',
         banner, '\n#include "internal.h"\n\n',
         '/* name, type, accent flags (1 the Dark value is the accent, 2 the Light one: only its alpha is kept), Dark, Light */\n',
         'const kst_res_def_t kst_res_table[K_RES_LAST_ - K_RES_FIRST_] = {\n']
    for e in entries:
        key, t, d, l, ad, al = e
        flags = (1 if ad else 0) | (2 if al else 0)
        n = snake(key)
        c.append('\t[%s - K_RES_FIRST_] = { "%s", KST_RT_%s, %d, { %s }, { %s } },\n' % (n, n, t, flags, val(t, d), val(t, l)))
    c.append('};\n')
    with open(os.path.join(root_dir, 'src', 'theme_resources.c'), 'w', newline='\n') as f:
        f.write(''.join(c))

    print('%d keys: %s; skipped: %s' % (len(entries), summary, skipped_text))


if __name__ == '__main__':
    main()
