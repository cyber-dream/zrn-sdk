# SPDX-License-Identifier: GPL-2.0-or-later
"""zrn: make, look at, package and install ztOS apps.

Every command takes --json: one JSON object on stdout, {"ok": true, ...} or
{"ok": false, "error": {"code", "message", "hint"}}. Exit codes: 0 done,
1 failed, 2 a package is not trusted (verify, install), 3 usage.
"""
import argparse
import glob
import json
import os
import platform
import re
import shlex
import shutil
import signal
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

from . import sign, zexec
from .agents import AGENT_NAMES
from .errors import ZrnError

VERSION = "0.1.0"
SDK = Path(__file__).resolve().parents[2]
TEMPLATES = SDK / "templates"
LANGS = ("c", "cpp", "python", "rust")


# ---------------------------------------------------------------- output ---
class Out:
    def __init__(self, as_json):
        self.json = as_json

    def say(self, *parts):
        if not self.json:
            print(*parts)

    def note(self, *parts):
        print(*parts, file=sys.stderr)


def result(out, data, text=None):
    if out.json:
        print(json.dumps({"ok": True, **data}, indent=1, sort_keys=True))
    elif text:
        print(text)


# ----------------------------------------------------------------- paths ---
def config_home():
    env = os.environ.get("ZRN_CONFIG_HOME")
    if env:
        return Path(env)
    return Path(os.environ.get("XDG_CONFIG_HOME") or Path.home() / ".config") / "zrn"


def data_home():
    return Path(os.environ.get("XDG_DATA_HOME") or Path.home() / ".local" / "share")


def cache_home():
    return Path(os.environ.get("XDG_CACHE_HOME") or Path.home() / ".cache") / "zrn"


def state_home():
    return Path(os.environ.get("XDG_STATE_HOME") or Path.home() / ".local" / "state") / "zrn"


def trusted_key_files(extra=()):
    dirs = [Path("/etc/zrn/trusted-keys"), Path("/usr/share/zrn/trusted-keys"), config_home() / "trusted-keys"]
    files = []
    for d in dirs:
        if d.is_dir():
            files += sorted(d.glob("*.pub"))
    for f in os.environ.get("ZRN_TRUSTED_KEYS", "").split(":"):
        if f:
            files.append(Path(f))
    files += [Path(f) for f in extra]
    return files


def load_keys(files):
    keys = []
    for f in files:
        try:
            keys.append(sign.PublicKey.loads(Path(f).read_text(), str(f)))
        except (OSError, sign.SignError) as e:
            raise ZrnError("bad-key", f"cannot read public key {f}: {e}",
                           "remove it, or replace it with a minisign public key")
    return keys


def host_arch():
    m = platform.machine()
    return {"arm64": "aarch64", "amd64": "x86_64"}.get(m, m)


# ------------------------------------------------------------- libzrnui ----
def _run(cmd, cwd=None, env=None, what="command"):
    p = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True, text=True)
    if p.returncode != 0:
        tail = (p.stderr or p.stdout).strip().splitlines()[-25:]
        raise ZrnError("build-failed", f"{what} failed (exit {p.returncode}): {' '.join(map(str, cmd))}\n"
                       + "\n".join(tail), "fix the first error above, then run the command again")
    return p


def zrnui_env():
    """How to compile and run against libzrnui: the system's, else the SDK's own build."""
    if subprocess.run(["pkg-config", "--exists", "zrnui"], capture_output=True).returncode == 0 \
            and not os.environ.get("ZRN_USE_SDK_TREE"):
        cflags = subprocess.run(["pkg-config", "--cflags", "zrnui"], capture_output=True, text=True).stdout.split()
        libs = subprocess.run(["pkg-config", "--libs", "zrnui"], capture_output=True, text=True).stdout.split()
        return {"cflags": cflags, "libs": libs, "libdir": None, "python": None, "system": True}
    lib = SDK / "libzrnui"
    built = lib / "build" / "libzrnui.so"
    sources = list((lib / "src").glob("*.[ch]")) + list((lib / "include").glob("*"))
    if not built.exists() or any(s.stat().st_mtime > built.stat().st_mtime for s in sources):
        if not (lib / "Makefile").exists():
            raise ZrnError("no-zrnui", "libzrnui is neither installed (pkg-config zrnui) nor in the SDK tree",
                           f"set ZRN_SDK to a zrn-sdk checkout, or install libzrnui")
        _run(["make", "-C", str(lib), "-j4"], what="building libzrnui")
    return {"cflags": [f"-I{lib / 'include'}"], "libs": [f"-L{lib / 'build'}", "-lzrnui"],
            "libdir": str(lib / "build"), "python": str(SDK / "python"), "system": False}


def run_env(z, extra=None):
    env = dict(os.environ)
    if z["libdir"]:
        env["LD_LIBRARY_PATH"] = z["libdir"] + (":" + env["LD_LIBRARY_PATH"] if env.get("LD_LIBRARY_PATH") else "")
        env["ZRNUI_LIB"] = str(Path(z["libdir"]) / "libzrnui.so.0")
    if z["python"]:
        env["PYTHONPATH"] = z["python"] + (":" + env["PYTHONPATH"] if env.get("PYTHONPATH") else "")
    env.update(extra or {})
    return env


# --------------------------------------------------------------- project ---
class Project:
    def __init__(self, root):
        import tomllib
        self.root = Path(root).resolve()
        path = self.root / "zrnapp.toml"
        try:
            self.data = tomllib.loads(path.read_text())
        except tomllib.TOMLDecodeError as e:
            raise ZrnError("project-toml", f"{path}: {e}", "fix the TOML syntax at that line")
        self.app = self.data.get("app", {})
        self.build = self.data.get("build", {})
        for k in ("id", "name", "version"):
            if not self.app.get(k):
                raise ZrnError("project-field", f"{path}: [app] {k} is missing", f'add {k} = "..." under [app]')
        self.id = self.app["id"]
        self.lang = self.build.get("lang", "")
        if self.lang not in LANGS:
            raise ZrnError("project-lang", f"{path}: [build] lang = {self.lang!r}",
                           f"use one of: {', '.join(LANGS)}")
        if not zexec.ID_RE.match(self.id):
            raise ZrnError("project-id", f"{path}: id {self.id!r} is not valid",
                           "use reverse-DNS with at least three parts (io.github.you.app) "
                           "or zrn-name for Zerone Laboratories' apps")

    @property
    def runtime(self):
        return "python3" if self.lang == "python" else "native"

    def arch(self, wanted=None):
        return "any" if self.runtime == "python3" else (wanted or host_arch())

    def out_dir(self, arch):
        return self.root / "build" / arch


def find_project(path):
    p = Path(path or ".").resolve()
    for d in [p, *p.parents]:
        if (d / "zrnapp.toml").exists():
            return Project(d)
    raise ZrnError("no-project", f"no zrnapp.toml in {p} or above",
                   "run zrn in an app's folder, or make one with `zrn new NAME`")


def build_project(proj, arch=None, out=None):
    arch = proj.arch(arch)
    if arch not in ("any", host_arch()):
        raise ZrnError("no-cross-compiler", f"cannot build for {arch} on this {host_arch()} machine yet",
                       "build on an aarch64 machine (the phone, or the store's CI, which builds on arm64 "
                       "runners); cross builds need aarch64-linux-gnu-gcc and an Arch Linux ARM sysroot")
    z = zrnui_env()
    dest = proj.out_dir(arch)
    (dest / "bin").mkdir(parents=True, exist_ok=True)
    binary = dest / "bin" / proj.id
    if proj.lang in ("c", "cpp"):
        sources = proj.build.get("sources") or []
        if not sources:
            raise ZrnError("no-sources", f"{proj.root / 'zrnapp.toml'}: [build] sources is empty",
                           'list the files, e.g. sources = ["main.c"]')
        missing = [s for s in sources if not (proj.root / s).exists()]
        if missing:
            raise ZrnError("source-missing", f"source file not found: {', '.join(missing)}",
                           f"create it, or fix [build] sources in {proj.root / 'zrnapp.toml'}")
        cc = os.environ.get("CXX" if proj.lang == "cpp" else "CC") or ("c++" if proj.lang == "cpp" else "cc")
        std = "-std=c++17" if proj.lang == "cpp" else "-std=gnu11"
        extra = []
        for pkg in proj.build.get("libs", []):
            p = subprocess.run(["pkg-config", "--cflags", "--libs", pkg], capture_output=True, text=True)
            if p.returncode:
                raise ZrnError("pkg-missing", f"pkg-config does not know {pkg!r}", f"install its -dev files, "
                               f"or remove it from [build] libs")
            extra += p.stdout.split()
        # the same commit gives the same bytes wherever it is built (no build paths inside)
        cmd = [cc, std, "-O2", "-g", "-Wall", f"-ffile-prefix-map={proj.root}=.", *proj.build.get("cflags", []), *z["cflags"],
               *[str(proj.root / s) for s in sources], "-o", str(binary), *z["libs"], *extra, "-lm"]
        _run(cmd, cwd=proj.root, what="compiling")
    elif proj.lang == "rust":
        if not shutil.which("cargo"):
            raise ZrnError("no-cargo", "cargo is not installed", "install Rust (pacman -S rust), then build again")
        import tomllib
        crate = tomllib.loads((proj.root / "Cargo.toml").read_text())["package"]["name"]
        env = run_env(z, {"ZRNUI_LIB_DIR": z["libdir"]} if z["libdir"] else {})
        env["RUSTFLAGS"] = (env.get("RUSTFLAGS", "") + f" --remap-path-prefix={proj.root}=.").strip()
        _run(["cargo", "build", "--release", "--quiet"], cwd=proj.root, env=env, what="cargo build")
        shutil.copy2(proj.root / "target" / "release" / crate, binary)
    else:  # python: nothing to compile, but every file must at least parse
        entry = proj.build.get("entry", "main.py")
        if not (proj.root / entry).exists():
            raise ZrnError("entry-missing", f"the entry {entry} does not exist", "fix [build] entry")
        for f in python_files(proj):
            try:
                compile((proj.root / f).read_bytes(), str(proj.root / f), "exec")
            except SyntaxError as e:
                raise ZrnError("python-syntax", f"{e.filename}:{e.lineno}: {e.msg}",
                               "fix the syntax error at that line")
        binary = None
    return {"arch": arch, "binary": str(binary) if binary else None, "lang": proj.lang, "zrnui": z}


def python_files(proj):
    pats = proj.build.get("include") or ["*.py"]
    found = set()
    for pat in pats:
        for f in glob.glob(pat, root_dir=proj.root, recursive=True):
            if (proj.root / f).is_file() and not f.startswith(("build/", "dist/")):
                found.add(f)
    entry = proj.build.get("entry", "main.py")
    found.add(entry)
    return sorted(found)


def manifest_for(proj, arch, bundled=False):
    a = proj.app
    m = {
        "zexec": zexec.FORMAT, "id": proj.id, "name": a["name"], "version": a["version"],
        "summary": a.get("summary", ""), "description": a.get("description", ""),
        "author": a.get("author", ""), "licence": a.get("licence", a.get("license", "")),
        "homepage": a.get("homepage", ""), "arch": arch, "runtime": proj.runtime,
        "entry": ("app/" + proj.build.get("entry", "main.py")) if proj.runtime == "python3" else f"bin/{proj.id}",
        "zrnui": "0.1", "min_ztos": a.get("min_ztos", "0.2.1"),
        "permissions": a.get("permissions", []), "categories": a.get("categories", []),
        "data_dirs": a.get("data_dirs", []),
        "built": {"by": f"zrn {VERSION}", "lang": proj.lang},
    }
    icon = a.get("icon")
    if icon:
        m["icon"] = "icon" + Path(icon).suffix
    if bundled:
        m["bundles"] = ["zrnui"]
    src = os.environ.get("ZRN_SOURCE_REPO"), os.environ.get("ZRN_SOURCE_COMMIT")
    if all(src):
        m["source"] = {"repo": src[0], "commit": src[1]}
    return m


def payload(proj, built, m, bundle=False):
    files = {}
    if m.get("icon"):
        src = proj.root / proj.app["icon"]
        if not src.exists():
            raise ZrnError("icon-missing", f"the icon {src} does not exist", "add it, or remove icon from [app]")
        files[m["icon"]] = (str(src), 0o644)
    if proj.runtime == "python3":
        for f in python_files(proj):
            files["app/" + f] = (str(proj.root / f), 0o644)
    else:
        files[m["entry"]] = (built["binary"], 0o755)
    for pat in proj.build.get("data", []):
        for f in glob.glob(pat, root_dir=proj.root, recursive=True):
            if (proj.root / f).is_file():
                files["share/" + f] = (str(proj.root / f), 0o644)
    if bundle:
        z = built["zrnui"]
        libdir = Path(z["libdir"] or "/usr/lib")
        real = (libdir / "libzrnui.so.0").resolve()
        files["lib/libzrnui.so.0"] = (str(real), 0o755)
        fonts = SDK / "libzrnui" / "fonts" if not z["system"] else Path("/usr/share/zrnui/fonts")
        for f in sorted(fonts.glob("*")):
            files["share/zrnui/fonts/" + f.name] = (str(f), 0o644)
        if proj.runtime == "python3":
            files["python/zrnui/__init__.py"] = (str(SDK / "python" / "zrnui" / "__init__.py"), 0o644)
    return files


def default_key(create=True, out=None):
    path = config_home() / "keys" / "dev.key"
    if not path.exists():
        if not create:
            raise ZrnError("no-key", f"no signing key at {path}", "make one with `zrn keygen`")
        make_key(path, out)
    return path


def make_key(path, out=None, trust=True):
    key = sign.SecretKey.generate()
    sign.write_secret(str(path), key)
    pub = path.with_suffix(".pub")
    pub.write_text(key.public.dumps())
    if trust:
        t = config_home() / "trusted-keys"
        t.mkdir(parents=True, exist_ok=True)
        (t / f"{path.stem}-{key.public.id_hex}.pub").write_text(key.public.dumps())
    if out:
        out.note(f"zrn: made signing key {key.public.id_hex} ({path}); this user now trusts it")
    return key


# -------------------------------------------------------------- commands ---
def cmd_new(args, out):
    name = args.name
    lang = args.lang
    slug = re.sub(r"[^a-z0-9]+", "-", name.lower()).strip("-") or "app"
    app_id = args.id or f"io.github.{re.sub(r'[^a-z0-9]', '', os.environ.get('USER', 'me').lower()) or 'me'}.{slug.replace('-', '')}"
    if not zexec.ID_RE.match(app_id):
        raise ZrnError("bad-id", f"{app_id!r} is not a valid app id",
                       "use --id io.github.you.app (reverse-DNS, three parts or more, lower case)")
    dest = Path(args.dir or slug).resolve()
    if dest.exists() and any(dest.iterdir()):
        raise ZrnError("exists", f"{dest} already exists and is not empty", "choose another name or --dir")
    src = TEMPLATES / lang
    crate = re.sub(r"[^a-z0-9_-]", "-", app_id.replace(".", "-"))
    subs = {"{{ID}}": app_id, "{{NAME}}": name, "{{CRATE}}": crate, "{{SDK}}": str(SDK)}
    made = []
    for f in sorted(src.rglob("*")):
        if f.is_dir():
            continue
        rel = f.relative_to(src)
        target = dest / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        text = f.read_text()
        for k, v in subs.items():
            text = text.replace(k, v)
        target.write_text(text)
        made.append(str(rel))
    (dest / "AGENTS.md").write_text((TEMPLATES / "agents" / "INSTRUCTIONS.md").read_text())
    (dest / ".gitignore").write_text((TEMPLATES / "gitignore").read_text())
    made += ["AGENTS.md", ".gitignore"]
    result(out, {"command": "new", "path": str(dest), "id": app_id, "lang": lang, "files": made},
           f"made {dest} ({lang}, id {app_id})\nnext: cd {shlex.quote(str(dest))} && zrn build && zrn render out.png")


def cmd_build(args, out):
    proj = find_project(args.project)
    built = build_project(proj, args.arch, out)
    result(out, {"command": "build", "id": proj.id, "arch": built["arch"], "lang": proj.lang,
                 "binary": built["binary"], "zrnui": "system" if built["zrnui"]["system"] else built["zrnui"]["libdir"]},
           f"built {proj.id} ({proj.lang}, {built['arch']})" + (f": {built['binary']}" if built["binary"] else ""))


def app_command(proj, built):
    if proj.runtime == "python3":
        return [sys.executable, str(proj.root / proj.build.get("entry", "main.py"))]
    return [built["binary"]]


def png_size(path):
    with open(path, "rb") as f:
        head = f.read(24)
    if head[:8] != b"\x89PNG\r\n\x1a\n":
        return None
    return struct.unpack(">II", head[16:24])


def cmd_render(args, out):
    proj = find_project(args.project)
    built = build_project(proj, None, out)
    path = Path(args.output).resolve()
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists():
        path.unlink()
    w, h = (int(v) for v in args.size.lower().split("x"))
    # ZUI_NO_OPEN: the page as it is once opened, not the first frame of the opening turn (nothing is there yet)
    env = {"ZUI_RENDER_PNG": str(path), "ZUI_RENDER_SIZE": f"{w}x{h}@{args.scale}", "ZUI_NO_OPEN": "1",
           "EGL_LOG_LEVEL": "fatal", "MESA_LOG_LEVEL": "error"}
    if args.page is not None:
        env["ZUI_RENDER_PAGE"] = str(args.page)
    if args.scroll:
        env["ZUI_RENDER_SCROLL"] = str(args.scroll)
    p = subprocess.run(app_command(proj, built), cwd=proj.root, env=run_env(built["zrnui"], env),
                       capture_output=True, text=True, timeout=60)
    if p.returncode == 2:
        raise ZrnError("no-page", (p.stderr.strip().splitlines() or ["no such page"])[-1],
                       "pass --page with a number from 0 to the number of pages minus one")
    if p.returncode != 0 or not path.exists():
        tail = "\n".join([l for l in p.stderr.splitlines() if "pci id" not in l][-15:])
        raise ZrnError("render-failed", f"the app did not draw {path} (exit {p.returncode})\n{tail}",
                       "the app must call zui_run()/app.run() with its first page; "
                       "offscreen drawing needs Mesa's EGL (surfaceless) or a /dev/dri render node")
    size = png_size(path)
    result(out, {"command": "render", "path": str(path), "pixels": list(size), "points": [w, h],
                 "scale": args.scale, "style": args.style or "default", "page": args.page},
           f"rendered {path} ({size[0]}x{size[1]} px)")


def find_compositor(headless):
    """A nested Wayland compositor: (name, argv builder)."""
    if headless:
        if shutil.which("kwin_wayland"):
            return "kwin_wayland", lambda sock, w, h: ["kwin_wayland", "--virtual", "--socket", sock,
                                                       "--width", str(w), "--height", str(h), "--no-lockscreen"]
        if shutil.which("weston"):
            return "weston", lambda sock, w, h: ["weston", "--backend=headless", f"--socket={sock}",
                                                 f"--width={w}", f"--height={h}"]
        return None, None
    if shutil.which("weston") and os.environ.get("DISPLAY"):
        return "weston", lambda sock, w, h: ["weston", "--backend=x11", f"--socket={sock}",
                                             f"--width={w}", f"--height={h}"]
    if shutil.which("kwin_wayland") and os.environ.get("DISPLAY"):
        return "kwin_wayland", lambda sock, w, h: ["kwin_wayland", "--x11-display", os.environ["DISPLAY"],
                                                   "--socket", sock, "--width", str(w), "--height", str(h),
                                                   "--no-lockscreen"]
    return None, None


def cmd_run(args, out):
    proj = find_project(args.project)
    built = build_project(proj, None, out)
    w, h = (int(v) for v in args.size.lower().split("x"))
    env = {"ZUI_WINDOW": f"{w}x{h}", "ZRN_APP_ID": proj.id}
    if args.scale:
        env["ZUI_SCALE"] = str(args.scale)
    if args.script:
        env["ZUI_SCRIPT"] = args.script
    runtime = os.environ.get("XDG_RUNTIME_DIR", f"/run/user/{os.getuid()}")
    own = os.environ.get("WAYLAND_DISPLAY")
    comp = None
    if own and Path(runtime, own).exists() and not args.headless:
        sock = own
    else:
        name, argv = find_compositor(args.headless)
        if not name:
            raise ZrnError("no-compositor", "no Wayland session and no compositor to run one in "
                           f"({'headless' if args.headless else 'windowed'})",
                           "install weston or kwin (kwin_wayland), or use `zrn render` to see a page without one")
        sock = f"zrn-run-{os.getpid()}"
        scale = int(args.scale or 1)
        cenv = dict(os.environ)
        cenv.pop("WAYLAND_DISPLAY", None)
        if args.headless:
            cenv.pop("DISPLAY", None)
        log = open(Path(tempfile.gettempdir()) / f"{sock}.log", "w")
        comp = subprocess.Popen(argv(sock, w * scale, h * scale), env=cenv, stdout=log, stderr=log,
                                start_new_session=True)
        for _ in range(100):
            if Path(runtime, sock).exists():
                break
            if comp.poll() is not None:
                raise ZrnError("compositor-failed", f"{name} exited at once (see {log.name})",
                               "try --headless, or `zrn render` instead")
            time.sleep(0.1)
        out.note(f"zrn: running {proj.id} in {name} ({w}x{h} points)")
    try:
        p = subprocess.run(app_command(proj, built), cwd=proj.root,
                           env=run_env(built["zrnui"], {**env, "WAYLAND_DISPLAY": sock}),
                           timeout=args.timeout or None)
        code = p.returncode
    except subprocess.TimeoutExpired:
        code = 124
    finally:
        if comp:
            os.killpg(comp.pid, signal.SIGTERM)
            try:
                comp.wait(5)
            except subprocess.TimeoutExpired:
                os.killpg(comp.pid, signal.SIGKILL)
    if code != 0:
        raise ZrnError("app-failed", f"{proj.id} exited with {code}", "read what it printed above")
    result(out, {"command": "run", "id": proj.id, "exit": code}, None)


def cmd_pack(args, out):
    proj = find_project(args.project)
    built = build_project(proj, args.arch, out)
    m = manifest_for(proj, built["arch"], args.bundle_zrnui)
    files = payload(proj, built, m, args.bundle_zrnui)
    dist = Path(args.out or proj.root / "dist")
    dist.mkdir(parents=True, exist_ok=True)
    path = dist / f"{proj.id}-{m['version']}-{m['arch']}.zexec"
    try:
        sha = zexec.pack(str(path), m, files)
    except zexec.ZexecError as e:
        raise ZrnError(e.code, e.message, e.hint)
    info = {"command": "pack", "path": str(path), "sha256": sha, "size": path.stat().st_size,
            "id": proj.id, "version": m["version"], "arch": m["arch"], "signed": False}
    if not args.no_sign:
        keyfile = Path(args.key) if args.key else default_key(True, out)
        try:
            key = sign.SecretKey.loads(keyfile.read_text(), str(keyfile))
        except sign.SignError as e:
            raise ZrnError("bad-key", str(e), "make a new one with `zrn keygen --force`")
        sig = sign.sign_file(key, str(path), zexec.trusted_comment(m, sha),
                             f"signature from zrn key {key.public.id_hex}")
        Path(str(path) + ".minisig").write_text(sig)
        info.update(signed=True, key=key.public.id_hex, signature=str(path) + ".minisig")
    result(out, info, f"packed {path} ({info['size']} bytes, sha256 {sha[:16]}...)"
           + (f", signed by {info.get('key')}" if info["signed"] else ", NOT signed"))


def verify_package(path, sig_path=None, extra_keys=(), require_signature=True):
    """(manifest, problems, signer or None, signature error or None, sha256)"""
    path = str(path)
    if not os.path.isfile(path):
        raise ZrnError("no-file", f"{path} does not exist", "give the path of a .zexec (zrn pack writes dist/*.zexec)")
    try:
        m, problems = zexec.check(path)
    except zexec.ZexecError as e:
        raise ZrnError(e.code, e.message, e.hint)
    sha = zexec.sha256_file(path)
    sig_path = Path(sig_path or path + ".minisig")
    signer, sig_error = None, None
    if not sig_path.exists():
        sig_error = f"no signature ({sig_path} is missing)"
    else:
        try:
            keys = load_keys(trusted_key_files(extra_keys))
            key, s = sign.verify_file(path, sig_path.read_text(), keys, str(sig_path))
            tc = zexec.parse_trusted_comment(s.trusted_comment)
            for field in ("id", "version", "arch"):
                if tc.get(field) != str(m.get(field)):
                    raise sign.SignError(f"the signature is for {field}={tc.get(field)!r}, "
                                         f"the package says {m.get(field)!r}")
            if tc.get("sha256") and tc["sha256"] != sha:
                raise sign.SignError("the signature names another file (sha256 differs)")
            signer = key.id_hex
        except sign.SignError as e:
            sig_error = str(e)
    return m, problems, signer, sig_error, sha


def cmd_verify(args, out):
    m, problems, signer, sig_error, sha = verify_package(args.file, args.sig, args.key or ())
    ok = not problems and not sig_error
    data = {"command": "verify", "path": str(Path(args.file).resolve()), "sha256": sha,
            "id": m.get("id") if isinstance(m, dict) else None, "version": m.get("version") if isinstance(m, dict) else None,
            "arch": m.get("arch") if isinstance(m, dict) else None,
            "valid": not problems, "problems": problems, "signed_by": signer, "signature_error": sig_error}
    if out.json:
        print(json.dumps({"ok": ok, **data}, indent=1, sort_keys=True))
    else:
        for pr in problems:
            print(f"problem: {pr}")
        if sig_error:
            print(f"signature: {sig_error}")
        if ok:
            print(f"good: {m['id']} {m['version']} ({m['arch']}), signed by {signer}")
    if problems:
        return 1
    if sig_error:
        return 2
    return 0


def tell_shell(line):
    runtime = os.environ.get("XDG_RUNTIME_DIR") or f"/run/user/{os.getuid()}"
    try:
        fd = os.open(os.path.join(runtime, "ztmetro.ctl"), os.O_WRONLY | os.O_NONBLOCK)
        os.write(fd, (line + "\n").encode())
        os.close(fd)
        return True
    except OSError:
        return False


def zrn_exec_path():
    found = shutil.which("zrn-exec")
    return found or str(SDK / "tools" / "zrn-exec")


def cmd_install(args, out):
    src = Path(args.file).resolve()
    m, problems, signer, sig_error, sha = verify_package(src, None, args.key or ())
    if problems:
        raise ZrnError("invalid-package", f"{src} is broken: " + "; ".join(problems), "rebuild it with `zrn pack`")
    if sig_error:
        raise ZrnError("untrusted", f"{src}: {sig_error}",
                       "install packages signed by a key this user trusts (~/.config/zrn/trusted-keys); "
                       "for your own builds `zrn pack` signs with your dev key", exit_code=2)
    if m["arch"] not in ("any", host_arch()):
        raise ZrnError("wrong-arch", f"{m['id']} is built for {m['arch']}, this is {host_arch()}",
                       "install the package built for this machine")
    app_dir = data_home() / "zrn" / "apps" / m["id"]
    app_dir.mkdir(parents=True, exist_ok=True)
    dest = app_dir / f"{m['id']}.zexec"
    shutil.copy2(src, dest)
    shutil.copy2(str(src) + ".minisig", str(dest) + ".minisig")
    icon_name = None
    if m.get("icon"):
        ext = Path(m["icon"]).suffix
        icon_dir = data_home() / "icons" / "hicolor" / ("scalable" if ext == ".svg" else "256x256") / "apps"
        icon_dir.mkdir(parents=True, exist_ok=True)
        with zexec.open_zexec(str(dest)) as z:
            (icon_dir / f"{m['id']}{ext}").write_bytes(z.read(m["icon"]))
        icon_name = m["id"]
    apps = data_home() / "applications"
    apps.mkdir(parents=True, exist_ok=True)
    desktop = apps / f"{m['id']}.desktop"
    cats = ";".join(m.get("categories") or ["Utility"]) + ";"
    desktop.write_text("\n".join([
        "[Desktop Entry]", "Type=Application", f"Name={m['name']}", f"Comment={m.get('summary') or m['name']}",
        f"Exec={zrn_exec_path()} {dest}", f"Icon={icon_name or 'application-x-executable'}", "Terminal=false",
        f"StartupWMClass={m['id']}", f"Categories={cats}", f"X-Zrn-Id={m['id']}",
        f"X-Zrn-Version={m['version']}", f"X-Zrn-Sha256={sha}", ""]))
    told = tell_shell("apps")
    result(out, {"command": "install", "id": m["id"], "version": m["version"], "path": str(dest),
                 "desktop": str(desktop), "signed_by": signer, "shell_told": told},
           f"installed {m['id']} {m['version']} -> {dest}\n  app list: {desktop}")


def stop_app(app_id):
    """Everything running from this app's unpacked package. Returns how many."""
    import signal, time
    root = str(cache_home() / "apps" / app_id) + "/"
    pids = []
    for name in os.listdir("/proc"):
        if not name.isdigit() or int(name) == os.getpid():
            continue
        try:
            if os.readlink(f"/proc/{name}/exe").startswith(root):
                pids.append(int(name))
        except OSError:
            pass
    for pid in pids:
        try:
            os.kill(pid, signal.SIGTERM)
        except OSError:
            pass
    for _ in range(20):
        if not any(Path(f"/proc/{p}").exists() for p in pids):
            break
        time.sleep(0.1)
    for pid in pids:
        try:
            os.kill(pid, signal.SIGKILL)
        except OSError:
            pass
    return len(pids)


# Folders shared by the system or by every app. Removal never touches these, whatever
# a package's manifest says.
RESERVED_DATA_NAMES = {"zrn", "applications", "icons", "mime", "fonts", "systemd", "ztmetro", "autostart",
                       "pulse", "pipewire", "dconf", "ssh", "gnupg", "keyrings", "waydroid", "Trash"}


def app_data_paths(app_id, extra=()):
    """What an app keeps: by its id, and the folder names its manifest declares."""
    names = [app_id]
    for n in extra:
        if re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._-]*", str(n)) and n not in RESERVED_DATA_NAMES:
            names.append(n)
    home = Path.home()
    roots = [data_home(),
             Path(os.environ.get("XDG_STATE_HOME") or home / ".local" / "state"),
             Path(os.environ.get("XDG_CONFIG_HOME") or home / ".config"),
             Path(os.environ.get("XDG_CACHE_HOME") or home / ".cache")]
    paths = [root / n for n in names for root in roots]
    paths += [cache_home() / "apps" / app_id, state_home() / f"{app_id}.log", state_home() / f"{app_id}.log.1"]
    return paths


def cmd_uninstall(args, out):
    app_id = args.id
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._-]*", app_id) or app_id in RESERVED_DATA_NAMES:
        raise ZrnError("bad-id", f"{app_id} is not an app id", "see zrn list")
    pkg = data_home() / "zrn" / "apps" / app_id / f"{app_id}.zexec"
    declared = []
    if pkg.exists():
        try:
            with zexec.open_zexec(str(pkg)) as z:
                declared = zexec.read_manifest(z).get("data_dirs") or []
        except (OSError, ZrnError, ValueError, KeyError):
            pass
    stopped = stop_app(app_id)
    removed = []
    targets = [data_home() / "zrn" / "apps" / app_id, data_home() / "applications" / f"{app_id}.desktop",
               *(data_home() / "icons" / "hicolor").glob(f"*/apps/{app_id}.*")]
    if not getattr(args, "keep_data", False):
        targets += app_data_paths(app_id, declared)
    else:
        targets += [cache_home() / "apps" / app_id]
    for p in targets:
        if p.is_dir() and not p.is_symlink():
            shutil.rmtree(p)
            removed.append(str(p))
        elif p.exists() or p.is_symlink():
            p.unlink()
            removed.append(str(p))
    if not removed:
        raise ZrnError("not-installed", f"{app_id} is not installed for this user", "see ~/.local/share/zrn/apps")
    tell_shell("apps")
    result(out, {"command": "uninstall", "id": app_id, "removed": removed, "stopped": stopped,
                 "data_kept": bool(getattr(args, "keep_data", False))}, f"removed {app_id}")


def cmd_keygen(args, out):
    path = Path(args.output) if args.output else config_home() / "keys" / f"{args.name}.key"
    if path.exists():
        if not args.force:
            raise ZrnError("key-exists", f"{path} already exists", "pass --force to replace it (old packages "
                           "then verify only while the old .pub stays trusted)")
        path.unlink()
    key = make_key(path, None, trust=not args.no_trust)
    result(out, {"command": "keygen", "secret": str(path), "public": str(path.with_suffix(".pub")),
                 "key_id": key.public.id_hex, "trusted": not args.no_trust},
           f"key {key.public.id_hex}\n  secret: {path} (keep it private, never commit it)\n"
           f"  public: {path.with_suffix('.pub')}")


def cmd_info(args, out):
    if not os.path.isfile(args.file):
        raise ZrnError("no-file", f"{args.file} does not exist", "give the path of a .zexec")
    try:
        with zexec.open_zexec(args.file) as z:
            m = zexec.read_manifest(z)
            files = [(i.filename, i.file_size) for i in z.infolist()]
    except zexec.ZexecError as e:
        raise ZrnError(e.code, e.message, e.hint)
    if out.json:
        print(json.dumps({"ok": True, "command": "info", "manifest": m,
                          "files": [{"path": f, "size": s} for f, s in files]}, indent=1, sort_keys=True))
    else:
        print(json.dumps(m, indent=1, sort_keys=True))
        for f, s in files:
            print(f"  {s:>9}  {f}")


def cmd_doctor(args, out):
    checks = []

    def check(name, ok, detail, fix=None, required=True):
        checks.append({"name": name, "ok": bool(ok), "detail": detail, "fix": None if ok else fix,
                       "required": required})

    py = sys.version_info
    check("python", py >= (3, 11), f"{py.major}.{py.minor}", "install Python 3.11 or newer")
    for tool, fix in (("cc", "pacman -S gcc"), ("pkg-config", "pacman -S pkgconf"), ("make", "pacman -S make")):
        check(tool, shutil.which(tool), shutil.which(tool) or "missing", fix)
    check("c++", shutil.which("c++"), shutil.which("c++") or "missing", "pacman -S gcc (for C++ apps)", False)
    check("cargo", shutil.which("cargo"), shutil.which("cargo") or "missing", "pacman -S rust (for Rust apps)", False)
    mods = "wayland-client wayland-egl egl glesv2 freetype2 harfbuzz fontconfig libpng xkbcommon gbm".split()
    missing = [m for m in mods if subprocess.run(["pkg-config", "--exists", m], capture_output=True).returncode]
    check("libraries", not missing, "all present" if not missing else "missing: " + " ".join(missing),
          "pacman -S wayland mesa freetype2 harfbuzz fontconfig libpng libxkbcommon")
    try:
        z = zrnui_env()
        check("libzrnui", True, "system" if z["system"] else z["libdir"])
    except ZrnError as e:
        check("libzrnui", False, e.message, e.hint)
    fonts = SDK / "libzrnui" / "fonts"
    check("fonts", any(fonts.glob("Selawik-*.ttf")) or Path("/usr/share/zrnui/fonts").is_dir(), str(fonts),
          "the SDK checkout is missing libzrnui/fonts")
    check("crypto", sign.HAVE_CRYPTOGRAPHY, "python-cryptography" if sign.HAVE_CRYPTOGRAPHY else
          "pure-Python Ed25519 (slower)", "pacman -S python-cryptography", False)
    key = config_home() / "keys" / "dev.key"
    check("dev key", key.exists(), str(key), "zrn keygen (zrn pack also makes one)", False)
    name, _ = find_compositor(True)
    check("headless compositor", name, name or "none", "pacman -S weston (or kwin)", False)
    renders = os.path.exists("/dev/dri") and any(Path("/dev/dri").glob("renderD*"))
    check("offscreen GPU", renders, "/dev/dri/renderD*" if renders else "no render node (Mesa surfaceless may "
          "still work with llvmpipe)", "install mesa", False)
    ok = all(c["ok"] for c in checks if c["required"])
    if out.json:
        print(json.dumps({"ok": ok, "command": "doctor", "checks": checks}, indent=1))
    else:
        for c in checks:
            mark = "ok  " if c["ok"] else ("MISS" if c["required"] else "opt ")
            print(f"{mark} {c['name']:<20} {c['detail']}" + (f"\n     fix: {c['fix']}" if c["fix"] else ""))
    return 0 if ok else 1


STORE_ENDPOINT = "https://ztos-app-submit.ztos.workers.dev/submit"


def _git(proj_root, *args):
    p = subprocess.run(["git", "-C", str(proj_root), *args], capture_output=True, text=True)
    return p.stdout.strip() if p.returncode == 0 else None


def cmd_submit(args, out):
    """The store entry for this app at one commit; sent only with --send."""
    proj = find_project(args.project)
    top = _git(proj.root, "rev-parse", "--show-toplevel")
    if not top:
        raise ZrnError("not-git", f"{proj.root} is not in a git repository",
                       "the store builds from a public git repository: push the app to one first")
    repo = args.repo or _git(proj.root, "remote", "get-url", "origin")
    if not repo:
        raise ZrnError("no-remote", "no --repo and no 'origin' remote", "pass --repo https://github.com/you/app")
    if repo.startswith("git@github.com:"):
        repo = "https://github.com/" + repo[len("git@github.com:"):]
    repo = re.sub(r"\.git$", "", repo)
    if not re.match(r"^https://github\.com/[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+$", repo):
        raise ZrnError("bad-repo", f"{repo!r} is not a public GitHub repository URL",
                       "round 1 of the store builds from github.com only: https://github.com/OWNER/REPO")
    commit = args.commit or _git(proj.root, "rev-parse", "HEAD")
    if not commit or not re.match(r"^[0-9a-f]{40}$", commit):
        raise ZrnError("bad-commit", f"{commit!r} is not a full commit SHA", "pass --commit with all 40 hex digits")
    if not args.commit and _git(proj.root, "status", "--porcelain"):
        raise ZrnError("dirty", "the working tree has changes that are not committed",
                       "commit (and push) them: the store builds exactly the commit you submit")
    subdir = os.path.relpath(proj.root, top)
    payload_ = {"id": proj.id, "version": proj.app["version"], "repo": repo, "commit": commit,
                "subdir": "" if subdir == "." else subdir, "name": proj.app["name"],
                "author": proj.app.get("author", ""), "contact": args.contact or "", "notes": args.notes or ""}
    if not args.send:
        result(out, {"command": "submit", "sent": False, "endpoint": args.endpoint, "entry": payload_},
               json.dumps(payload_, indent=1) + f"\nnot sent; check it, push commit {commit[:12]}, "
               f"then run again with --send (to {args.endpoint})")
        return 0
    import urllib.request
    req = urllib.request.Request(args.endpoint, data=json.dumps(payload_).encode(),
                                 headers={"Content-Type": "application/json", "User-Agent": f"zrn/{VERSION}"})
    try:
        with urllib.request.urlopen(req, timeout=30) as r:
            answer = json.loads(r.read() or b"{}")
    except Exception as e:
        raise ZrnError("submit-failed", f"the store did not take the entry: {e}", "check the network and try again")
    result(out, {"command": "submit", "sent": True, "answer": answer, "entry": payload_},
           f"sent: {proj.id} {proj.app['version']} at {commit[:12]}; moderators review it on Discord")


def cmd_store(args, out):
    """The ztOS store on this phone: only the store's key counts here."""
    from . import store
    if args.action == "refresh":
        r = store.refresh()
        result(out, {"command": "store", **r}, f"index: {r['index']}; disabled: {', '.join(r['disabled']) or 'nothing'}")
    elif args.action == "list":
        r = store.listing(host_arch())
        result(out, {"command": "store", **r}, "\n".join(f"{a['id']} {a['version']} {a['installed'] or '-'}" for a in r["apps"]) or "nothing listed")
    else:
        if not args.id:
            raise ZrnError("usage", "zrn store install needs an app id", "`zrn store list` shows them", exit_code=3)
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            path = store.fetch_package(args.id, host_arch(), d)
            args.file, args.key = str(path), [os.environ.get("ZRN_STORE_KEY") or store.SYSTEM_KEY]
            return cmd_install(args, out)


def cmd_exec(args, out):
    from .launcher import launch
    return launch(args.file, args.args)


def cmd_init_agents(args, out):
    from .agents import init_agents
    return init_agents(args, out)


# ------------------------------------------------------------------ main ---
def parser():
    common = argparse.ArgumentParser(add_help=False)
    # SUPPRESS: given before or after the command, neither copy overwrites the other
    common.add_argument("--json", action="store_true", default=argparse.SUPPRESS,
                        help="machine-readable output on stdout")
    common.add_argument("-C", "--project", default=argparse.SUPPRESS,
                        help="the app's folder (default: here, or above)")
    ap = argparse.ArgumentParser(prog="zrn", description="Make, look at, package and install ztOS apps.",
                                 parents=[common])
    ap.add_argument("--version", action="version", version=f"zrn {VERSION}")
    sub = ap.add_subparsers(dest="cmd", required=True, metavar="COMMAND")

    p = sub.add_parser("new", parents=[common], help="a new app from a template")
    p.add_argument("name")
    p.add_argument("--lang", choices=LANGS, default="c")
    p.add_argument("--id", help="reverse-DNS id (default io.github.$USER.name)")
    p.add_argument("--dir")
    p.set_defaults(fn=cmd_new)

    p = sub.add_parser("build", parents=[common], help="compile the app")
    p.add_argument("--arch")
    p.set_defaults(fn=cmd_build)

    p = sub.add_parser("render", parents=[common], help="draw a page into a PNG (no display needed)")
    p.add_argument("output")
    p.add_argument("--page", help="number, in the order the app makes pages (0 is the first)")
    p.add_argument("--size", default="360x740", help="points, WxH (the S9 is 360x740)")
    p.add_argument("--scale", type=int, default=2, help="pixels per point (the S9 is 4)")
    p.add_argument("--scroll", type=float, help="scroll the page down this many points first")
    p.set_defaults(fn=cmd_render)

    p = sub.add_parser("run", parents=[common], help="run in a phone-sized Wayland window")
    p.add_argument("--headless", action="store_true", help="no window: a virtual compositor")
    p.add_argument("--script", help='steps: "wait 500; tap 180 300; type hi; snap a.png; back; quit"')
    p.add_argument("--size", default="360x740")
    p.add_argument("--scale", type=int)
    p.add_argument("--timeout", type=float)
    p.set_defaults(fn=cmd_run)

    p = sub.add_parser("pack", parents=[common], help="build and package as a signed .zexec")
    p.add_argument("--arch")
    p.add_argument("--out", help="folder (default dist/)")
    p.add_argument("--key", help="minisign secret key (default ~/.config/zrn/keys/dev.key)")
    p.add_argument("--no-sign", action="store_true")
    p.add_argument("--bundle-zrnui", action="store_true",
                   help="put libzrnui (and its fonts) inside, for systems without it")
    p.set_defaults(fn=cmd_pack)

    p = sub.add_parser("verify", parents=[common], help="check a .zexec and its signature")
    p.add_argument("file")
    p.add_argument("--sig", help="signature file (default FILE.minisig)")
    p.add_argument("--key", action="append", help="also trust this public key (repeatable)")
    p.set_defaults(fn=cmd_verify)

    p = sub.add_parser("install", parents=[common], help="install a .zexec for this user")
    p.add_argument("file")
    p.add_argument("--key", action="append", help="also trust this public key")
    p.set_defaults(fn=cmd_install)

    p = sub.add_parser("store", parents=[common], help="the ztOS store: refresh, list, install ID")
    p.add_argument("action", choices=("refresh", "list", "install"))
    p.add_argument("id", nargs="?")
    p.set_defaults(fn=cmd_store)

    p = sub.add_parser("uninstall", parents=[common], help="remove an installed app")
    p.add_argument("id")
    p.add_argument("--keep-data", action="store_true", help="leave what the app stored")
    p.set_defaults(fn=cmd_uninstall)

    p = sub.add_parser("keygen", parents=[common], help="make a signing key")
    p.add_argument("--name", default="dev")
    p.add_argument("--output", help="where the secret key goes")
    p.add_argument("--force", action="store_true")
    p.add_argument("--no-trust", action="store_true", help="do not add it to this user's trusted keys")
    p.set_defaults(fn=cmd_keygen)

    p = sub.add_parser("info", parents=[common], help="show a .zexec's manifest and files")
    p.add_argument("file")
    p.set_defaults(fn=cmd_info)

    p = sub.add_parser("doctor", parents=[common], help="check the toolchain")
    p.set_defaults(fn=cmd_doctor)

    p = sub.add_parser("submit", parents=[common], help="the store entry for this commit (sent with --send)")
    p.add_argument("--repo", help="public repository URL (default: the origin remote)")
    p.add_argument("--commit", help="full commit SHA (default: HEAD, which must be clean)")
    p.add_argument("--contact", help="how moderators reach you")
    p.add_argument("--notes", help="anything moderators should know")
    p.add_argument("--endpoint", default=STORE_ENDPOINT)
    p.add_argument("--send", action="store_true", help="really send it (default: only show it)")
    p.set_defaults(fn=cmd_submit)

    p = sub.add_parser("exec", parents=[common], help="run a .zexec (what zrn-exec does)")
    p.add_argument("file")
    p.add_argument("args", nargs=argparse.REMAINDER)
    p.set_defaults(fn=cmd_exec)

    p = sub.add_parser("init-agents", parents=[common],
                       help="instructions and the zrnui skill for coding agents in this project")
    p.add_argument("--agent", help="comma-separated: " + ", ".join(AGENT_NAMES))
    p.add_argument("--all", action="store_true")
    p.add_argument("--list", action="store_true", help="list the agents and where their files go")
    p.add_argument("--force", action="store_true", help="replace files that exist")
    p.set_defaults(fn=cmd_init_agents)
    return ap


def main(argv=None):
    ap = parser()
    try:
        args = ap.parse_args(argv)
    except SystemExit as e:
        return 3 if e.code not in (0, None) else 0
    args.json = getattr(args, "json", False)
    args.project = getattr(args, "project", None)
    out = Out(args.json)
    try:
        code = args.fn(args, out)
        return code or 0
    except ZrnError as e:
        if out.json:
            print(json.dumps({"ok": False, "error": {"code": e.code, "message": e.message, "hint": e.hint}},
                             indent=1))
        else:
            print(f"zrn: error: {e.message}", file=sys.stderr)
            if e.hint:
                print(f"zrn: fix: {e.hint}", file=sys.stderr)
        return e.exit_code
    except KeyboardInterrupt:
        return 130
    except OSError as e:
        msg = f"{e.filename}: {e.strerror}" if e.filename else str(e)
        if out.json:
            print(json.dumps({"ok": False, "error": {"code": "os-error", "message": msg,
                                                     "hint": "check the path and its permissions"}}, indent=1))
        else:
            print(f"zrn: error: {msg}", file=sys.stderr)
        return 1
