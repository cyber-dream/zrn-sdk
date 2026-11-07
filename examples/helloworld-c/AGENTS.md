# This is a ztOS app

ztOS is a Linux phone OS (Arch Linux ARM, Wayland) for the Samsung Galaxy S9.
This project builds one app with **libzrnui**, the ztOS app toolkit, and ships
it as a single `.zexec` file. The `zrnui` skill in this project has the full
API and the style rules; read it before writing UI code.

## Rules that always hold

- **Use only libzrnui for UI.** No Qt, GTK, SDL or web views. libzrnui draws on
  the GPU; do not draw pixels yourself.
- **Describe pages, do not position things.** Put widgets in pages, sections,
  cards, columns and rows; libzrnui lays them out, scrolls them and animates
  them. There are no x/y coordinates in app code.
- **The system picks the style, Metro or DotMatrix.** Do not hard-code colours, fonts or
  sizes; the accent colour comes from the system.
- **One thread for UI.** Long work (network, files, subprocesses) goes in a
  thread or a subprocess; hand results back with `post()` / `zui_post()`.
- **The app id** in `zrnapp.toml` is reverse-DNS (`io.github.you.app`) or
  `zrn-name` for Zerone Laboratories' own apps. Never change it after release.
- Declare every permission the app needs in `zrnapp.toml` (`permissions`).

## Commands (add `--json` for machine-readable output)

| what | command |
|---|---|
| check the toolchain | `zrn doctor` |
| build | `zrn build` |
| look at a screen (no display needed) | `zrn render --page 0 out.png` then open the PNG |
| the same in DotMatrix | `zrn render --style dotmatrix out.png` |
| a page further down | `zrn render --scroll 400 out.png` |
| run in a phone-sized window | `zrn run` (`--headless --script "wait 500; tap 180 300; snap a.png; quit"` without a screen) |
| package | `zrn pack` (writes `dist/*.zexec` and `.minisig`) |
| check a package | `zrn verify dist/*.zexec` |
| install for this user | `zrn install dist/*.zexec` |

Always run `zrn build` and `zrn render` after a UI change and look at the PNG
before saying the change is done.
