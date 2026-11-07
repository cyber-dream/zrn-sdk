# zrn-sdk: make (libzrnui), make check (all tests), make examples (.zexec of
# each example), make renders (docs/renders). At most -j4 on the laptop.
ZRN = tools/zrn
TESTBIN = build/tests
EXAMPLES = hello-c helloworld-c settings-c hello-cpp hello-py hello-rust

all:
	$(MAKE) -C libzrnui -j4

$(TESTBIN)/test_core: tests/test_core.c all
	@mkdir -p $(TESTBIN)
	cc -O1 -g -Wall -Ilibzrnui/include tests/test_core.c -Llibzrnui/build -lzrnui -lpng -lm \
		-Wl,-rpath,$(CURDIR)/libzrnui/build -o $@

check: $(TESTBIN)/test_core
	EGL_LOG_LEVEL=fatal $(TESTBIN)/test_core $(TESTBIN)
	python3 -I tests/test_tools.py
	python3 -I tests/test_store_client.py
	cd rust && cargo test --offline --quiet
	c++ -std=c++17 -fsyntax-only -Wall -Wextra -Ilibzrnui/include examples/hello-cpp/main.cpp

# run: the settings demo in a window on this desktop (a Wayland session; no
# virtual machine, no nested compositor). make run
# It is also what CLion's "run" configuration for this Makefile target does.
ARCH = $(shell uname -m)
# the window the run targets open, in points: WxH (make run WINDOW=360x740 for the phone)
WINDOW = 480x800
DEMO = examples/settings-c
run: all
	$(ZRN) -C $(DEMO) build
	ZUI_WINDOW=$(WINDOW) LD_LIBRARY_PATH=$(CURDIR)/libzrnui/build \
		$(DEMO)/build/$(ARCH)/bin/zrn-example-settings

# run-helloworld, run-hello: the other C examples, the same way.
run-helloworld: all
	$(ZRN) -C examples/helloworld-c build
	ZUI_WINDOW=$(WINDOW) LD_LIBRARY_PATH=$(CURDIR)/libzrnui/build \
		examples/helloworld-c/build/$(ARCH)/bin/zrn-example-helloworld

run-hello: all
	$(ZRN) -C examples/hello-c build
	ZUI_WINDOW=$(WINDOW) LD_LIBRARY_PATH=$(CURDIR)/libzrnui/build \
		examples/hello-c/build/$(ARCH)/bin/zrn-example-hello

examples: all
	for e in $(EXAMPLES); do $(ZRN) -C examples/$$e pack || exit 1; done

R = docs/renders
renders: all
	@mkdir -p $(R)
	$(ZRN) -C examples/settings-c render --scale 4 $(R)/metro-settings.png
	$(ZRN) -C examples/settings-c render --scale 4 --page 0 $(R)/metro-wifi.png
	$(ZRN) -C examples/hello-c render --scale 2 $(R)/hello-c.png
	$(ZRN) -C examples/hello-cpp render --scale 2 $(R)/hello-cpp.png
	$(ZRN) -C examples/hello-py render --scale 2 $(R)/hello-py.png
	$(ZRN) -C examples/hello-rust render --scale 2 $(R)/hello-rust.png
	$(ZRN) -C examples/hello-c render --scale 2 --style metro $(R)/hello-c-metro.png

# Everything a phone needs to run .zexec apps: libzrnui (library, pkg-config,
# headers, share/zrnui), and the tools in lib/zrn-sdk with bin/zrn and bin/zrn-exec.
PREFIX ?= /usr/local
install:
	$(MAKE) -C libzrnui -j4 PREFIX=$(PREFIX)
	$(MAKE) -C libzrnui install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)
	install -d $(DESTDIR)$(PREFIX)/lib/zrn-sdk/tools/zrnlib $(DESTDIR)$(PREFIX)/bin
	install -m755 tools/zrn tools/zrn-exec $(DESTDIR)$(PREFIX)/lib/zrn-sdk/tools/
	install -m644 tools/zrnlib/*.py $(DESTDIR)$(PREFIX)/lib/zrn-sdk/tools/zrnlib/
	cp -r templates $(DESTDIR)$(PREFIX)/lib/zrn-sdk/
	ln -sf ../lib/zrn-sdk/tools/zrn $(DESTDIR)$(PREFIX)/bin/zrn
	ln -sf ../lib/zrn-sdk/tools/zrn-exec $(DESTDIR)$(PREFIX)/bin/zrn-exec

clean:
	$(MAKE) -C libzrnui clean
	rm -rf build

.PHONY: all check examples renders clean install run run-helloworld run-hello
