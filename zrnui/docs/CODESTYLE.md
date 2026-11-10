# Code Style (zrnui)

## Language
- С23 (`-std=c23`), CMake project.
- No C++ features.

## Naming
- Public API: `Z` prefix, CamelCase (`Zroot_create`, `Zid`).
- Constants: `Z_` prefix (`true`, `false`, `Z_EVENT_CLICK`).
- Internal/static: `zrn_` or `zst_` prefixes, snake_case.
- Types: `_t` suffix (`zrn_root_t`, `glyph_entry_t`).

## Formatting
- Tabs for indentation.
- Braces on same line for control statements.
- One statement per line; no chained declarations where unclear.

## Booleans
- Use `bool` type from `stdbool.h`
- Name variables as b_is_something

# Integers
- Use uint and 8/16/32 bit size where it appreciatable

# If
- Use inverted if() to increase code readability

# Const
- Use const in func parameters and variables in code

## Memory
- Manual allocation (`calloc`/`malloc`/`free`).
- Check allocation results and report via `zrn_error`.

## Errors
- Use `zrn_error(...)` for runtime errors; return invalid IDs/`false`.

## Includes
- `internal.h` first in `.c`, then system headers.
- Guard headers with `#pragma once` or include guards.

## Comments
- Doxygen-style `/** ... */` for public API.
- Non english comments not allowed; 
