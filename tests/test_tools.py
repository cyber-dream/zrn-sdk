# SPDX-License-Identifier: GPL-2.0-or-later
"""The zrn tools and the Python binding: signatures, .zexec, the CLI's contract."""
import io
import json
import os
import secrets
import subprocess
import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "python"))

from zrnlib import ed25519, sign, zexec  # noqa: E402

ZRN = str(ROOT / "tools" / "zrn")


class Ed25519(unittest.TestCase):
    def test_rfc8032_vector_1(self):
        seed = bytes.fromhex("9d61b19deffd5a60ba844af492ec2cc44449c5697b326919703bac031cae7f60")
        pk = bytes.fromhex("d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a")
        sig = bytes.fromhex("e5564300c360ac729086e2cc806e828a84877f1eb8e5d974d873e06522490155"
                            "5fb8821590a33bacc61e39701cf9b46bd25bf5f0595bbe24655141438e7a100b")
        self.assertEqual(ed25519.public_key(seed), pk)
        self.assertEqual(ed25519.sign(seed, b""), sig)
        self.assertTrue(ed25519.verify(pk, b"", sig))
        self.assertFalse(ed25519.verify(pk, b"x", sig))

    @unittest.skipUnless(sign.HAVE_CRYPTOGRAPHY, "python-cryptography not installed")
    def test_pure_matches_cryptography(self):
        for _ in range(3):
            seed, msg = secrets.token_bytes(32), secrets.token_bytes(100)
            self.assertEqual(ed25519.public_key(seed), sign._ed_public(seed))
            self.assertEqual(ed25519.sign(seed, msg), sign._ed_sign(seed, msg))


class Minisign(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.TemporaryDirectory()
        self.path = os.path.join(self.dir.name, "f.bin")
        Path(self.path).write_bytes(b"hello " * 1000)
        self.key = sign.SecretKey.generate()

    def tearDown(self):
        self.dir.cleanup()

    def test_key_files_round_trip(self):
        k2 = sign.SecretKey.loads(self.key.dumps())
        self.assertEqual(k2.seed, self.key.seed)
        self.assertEqual(sign.PublicKey.loads(self.key.public.dumps()).pk, self.key.public.pk)

    def test_sign_verify_and_tamper(self):
        text = sign.sign_file(self.key, self.path, "zexec id=a version=1")
        key, sig = sign.verify_file(self.path, text, [self.key.public])
        self.assertEqual(sig.trusted_comment, "zexec id=a version=1")
        # another key: refused, saying which key signed it
        with self.assertRaisesRegex(sign.SignError, "not trusted"):
            sign.verify_file(self.path, text, [sign.SecretKey.generate().public])
        # the trusted comment changed
        lines = text.splitlines()
        lines[2] = "trusted comment: zexec id=b version=1"
        with self.assertRaisesRegex(sign.SignError, "altered"):
            sign.verify_file(self.path, "\n".join(lines) + "\n", [self.key.public])
        # one byte of the file changed
        data = bytearray(Path(self.path).read_bytes())
        data[10] ^= 1
        Path(self.path).write_bytes(bytes(data))
        with self.assertRaisesRegex(sign.SignError, "does not match"):
            sign.verify_file(self.path, text, [self.key.public])


MANIFEST = {"zexec": 1, "id": "zrn-test-app", "name": "Test", "version": "1.0.0", "arch": "any",
            "runtime": "python3", "entry": "app/main.py", "zrnui": "0.1"}


class Zexec(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.TemporaryDirectory()
        self.d = self.dir.name

    def tearDown(self):
        self.dir.cleanup()

    def test_reproducible(self):
        files = {"app/main.py": (b"print('hi')\n", 0o644), "app/b.py": (b"x = 1\n", 0o644)}
        a = zexec.pack(os.path.join(self.d, "a.zexec"), MANIFEST, files)
        b = zexec.pack(os.path.join(self.d, "b.zexec"), MANIFEST, dict(reversed(list(files.items()))))
        self.assertEqual(a, b)
        m, problems = zexec.check(os.path.join(self.d, "a.zexec"))
        self.assertEqual(problems, [])
        with open(os.path.join(self.d, "a.zexec"), "rb") as f:
            self.assertEqual(f.read(24), zexec.HEADER)

    def test_manifest_rules(self):
        bad = dict(MANIFEST, id="Bad Id", version="1", permissions=["root"], arch="x86_64")
        problems = " | ".join(zexec.validate_manifest(bad))
        for word in ("'id'", "'version'", "permission 'root'", "python3 app's 'arch'"):
            self.assertIn(word, problems)
        self.assertEqual(zexec.validate_manifest(dict(MANIFEST, id="io.github.alice.notes")), [])
        self.assertTrue(zexec.validate_manifest(dict(MANIFEST, id="com.notes")))  # two parts only

    def test_hostile_archive(self):
        path = os.path.join(self.d, "evil.zexec")
        buf = io.BytesIO()
        buf.write(zexec.HEADER)
        with zipfile.ZipFile(buf, "w") as z:
            z.writestr("zexec.json", json.dumps(MANIFEST))
            z.writestr("app/main.py", "pass\n")
            z.writestr("../../.bashrc", "evil\n")
        Path(path).write_bytes(buf.getvalue())
        _, problems = zexec.check(path)
        self.assertTrue(any("outside" in p for p in problems), problems)
        dest = os.path.join(self.d, "out")
        zexec.extract(path, dest)
        self.assertFalse(os.path.exists(os.path.join(self.d, ".bashrc")))


def zrn(*args, env=None, cwd=None):
    p = subprocess.run([sys.executable, ZRN, *args], capture_output=True, text=True, env=env, cwd=cwd)
    return p.returncode, p.stdout, p.stderr


class Cli(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.TemporaryDirectory()
        d = self.dir.name
        self.env = dict(os.environ, ZRN_CONFIG_HOME=f"{d}/cfg", XDG_DATA_HOME=f"{d}/data",
                        XDG_CACHE_HOME=f"{d}/cache", XDG_STATE_HOME=f"{d}/state", XDG_RUNTIME_DIR=d)

    def tearDown(self):
        self.dir.cleanup()

    def test_json_error_contract(self):
        code, out, _ = zrn("build", "--json", "-C", self.dir.name, env=self.env)
        self.assertEqual(code, 1)
        err = json.loads(out)["error"]
        self.assertEqual(err["code"], "no-project")
        self.assertIn("zrn new", err["hint"])

    def test_new_pack_verify_install(self):
        d = self.dir.name
        code, out, err = zrn("new", "Notes", "--lang", "python", "--id", "io.github.test.notes",
                             "--dir", f"{d}/notes", "--json", env=self.env)
        self.assertEqual(code, 0, err)
        self.assertTrue(Path(d, "notes", "AGENTS.md").exists())
        code, out, err = zrn("pack", "--json", "-C", f"{d}/notes", env=self.env)
        self.assertEqual(code, 0, err)
        info = json.loads(out)
        self.assertTrue(info["signed"])
        pkg = info["path"]
        code, out, _ = zrn("verify", pkg, "--json", env=self.env)
        self.assertEqual(code, 0, out)
        # someone else's trust store: valid, but not trusted (exit 2)
        other = dict(self.env, ZRN_CONFIG_HOME=f"{d}/other")
        code, out, _ = zrn("verify", pkg, "--json", env=other)
        self.assertEqual(code, 2)
        self.assertIn("not trusted", json.loads(out)["signature_error"])
        code, out, err = zrn("install", pkg, "--json", env=self.env)
        self.assertEqual(code, 0, err)
        desktop = Path(json.loads(out)["desktop"]).read_text()
        self.assertIn("Exec=", desktop)
        self.assertIn("StartupWMClass=io.github.test.notes", desktop)
        # a changed byte: refused
        data = bytearray(Path(pkg).read_bytes())
        data[-30] ^= 0xff
        Path(pkg).write_bytes(bytes(data))
        code, out, _ = zrn("verify", pkg, "--json", env=self.env)
        self.assertNotEqual(code, 0)

    def test_init_agents_only_in_an_app(self):
        code, out, _ = zrn("init-agents", "--all", "--json", "-C", self.dir.name, env=self.env)
        self.assertEqual(code, 1)
        self.assertEqual(json.loads(out)["error"]["code"], "no-project")


class Binding(unittest.TestCase):
    def test_python_callbacks(self):
        import zrnui
        app = zrnui.App(style="metro")
        page = app.page()
        seen = []
        b = page.button("Tap", lambda: seen.append("tap"))
        sw = page.section("S").switch("Wi-Fi", False, on_change=lambda on: seen.append(on))
        app.layout(page)
        for node in (b, sw):
            x, y, w, h = node.frame()
            app.tap(x + w / 2, y + h / 2)
        self.assertEqual(seen, ["tap", True])
        self.assertTrue(sw.on)


if __name__ == "__main__":
    unittest.main()
