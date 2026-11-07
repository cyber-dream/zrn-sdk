# libzrnui: make (-j4 at most on the laptop), make install PREFIX=/usr
PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
CC ?= cc
PKGS = wayland-client wayland-egl egl glesv2 freetype2 harfbuzz fontconfig libpng xkbcommon gbm
CFLAGS ?= -O2 -g
CFLAGS += -std=gnu11 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers \
	-fPIC -fvisibility=hidden -Iinclude -Iprotocol -Isrc $(shell pkg-config --cflags $(PKGS)) \
	-DZUI_DATADIR=\"$(PREFIX)/share/zrnui\"
LDLIBS = $(shell pkg-config --libs $(PKGS)) -lm -lpthread -ldl
VERSION = 0.1.0
SONAME = libzrnui.so.0
B = build
SRC = src/core.c src/class.c src/brush.c src/gfx.c src/text.c src/icons.c src/style-metro.c src/wl.c \
	src/extras.c src/offscreen.c protocol/xdg-shell-protocol.c
SRC += $(wildcard src/widgets/*.c)
OBJ = $(SRC:%.c=$(B)/%.o)

all: $(B)/libzrnui.so

$(B)/%.o: %.c src/zui-internal.h src/widgets/widgets.h include/zrnui.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(B)/libzrnui.so.$(VERSION): $(OBJ)
	$(CC) -shared -Wl,-soname,$(SONAME) -Wl,--no-undefined -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

$(B)/libzrnui.so: $(B)/libzrnui.so.$(VERSION)
	ln -sf libzrnui.so.$(VERSION) $(B)/$(SONAME)
	ln -sf libzrnui.so.$(VERSION) $@

$(B)/zrnui.pc: Makefile
	@mkdir -p $(B)
	printf 'prefix=%s\nlibdir=%s\nincludedir=%s/include\n\nName: zrnui\nDescription: ztOS app toolkit (GLES2, Metro and DotMatrix)\nVersion: %s\nRequires.private: wayland-client wayland-egl egl glesv2 freetype2 harfbuzz fontconfig libpng xkbcommon gbm\nLibs: -L$${libdir} -lzrnui\nCflags: -I$${includedir}\n' \
		'$(PREFIX)' '$(LIBDIR)' '$(PREFIX)' '$(VERSION)' > $@

install: all $(B)/zrnui.pc
	install -Dm755 $(B)/libzrnui.so.$(VERSION) $(DESTDIR)$(LIBDIR)/libzrnui.so.$(VERSION)
	ln -sf libzrnui.so.$(VERSION) $(DESTDIR)$(LIBDIR)/$(SONAME)
	ln -sf libzrnui.so.$(VERSION) $(DESTDIR)$(LIBDIR)/libzrnui.so
	install -Dm644 include/zrnui.h $(DESTDIR)$(PREFIX)/include/zrnui.h
	install -Dm644 include/zrnui.hpp $(DESTDIR)$(PREFIX)/include/zrnui.hpp
	install -Dm644 $(B)/zrnui.pc $(DESTDIR)$(LIBDIR)/pkgconfig/zrnui.pc
	install -Dm644 -t $(DESTDIR)$(PREFIX)/share/zrnui/fonts fonts/*.ttf fonts/*.txt

# for the IDE, which has this directory as its Makefile project: the examples
# run from the repository's Makefile (make run STYLE=metro|dotmatrix WINDOW=WxH)
run run-helloworld run-hello:
	$(MAKE) -C .. $@

clean:
	rm -rf $(B)

.PHONY: all install clean run run-helloworld run-hello
