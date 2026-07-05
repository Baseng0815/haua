# HAUA — Home Automation Unified Architecture

A from-scratch smart home stack where every node talks OPC UA. No accounts, no
cloud, no bluetooth pairing dances — just local, industrial-grade reliability.
The first subsystem under development is LED lighting (`led/`).

## Repository layout

- `led/` — the LED lighting subsystem
  - `generic/` — platform-independent core: the HAL (color, strip types),
    effects, and anything common to every LED strip
  - `esp32/` — concrete strip drivers for ESP32 targets (e.g. `ws2805`), built
    with the ESP-IDF toolchain, not part of the desktop build
  - `x86/` — concrete strip drivers that run on a desktop
    - `mock/` — a mock LED strip that renders to the terminal, for development
      without hardware
- `external/open62541/` — vendored OPC UA stack (git submodule)
- `flake.nix` — Nix dev shell providing clang, cmake, ninja, python, mbedtls

## Architecture

The `generic` library owns the hardware abstraction: `struct color_rgb`,
`struct strip_info`, `struct strip_state`, effects, and time types. A concrete
strip module (esp32, x86/mock) depends on `haua::led_generic`, allocates a
`strip_state`, and implements the actual output for its platform. The mock
implements output by drawing the strip with ANSI truecolor blocks.

## Build & run

The toolchain lives in the Nix dev shell:

```sh
nix develop                 # enter the dev shell
cmake -B build -G Ninja     # configure (first time)
cmake --build build         # build everything
./build/bin/led_mock        # run the mock strip (Ctrl+C to quit)
```

`compile_commands.json` is exported for clangd.

## Conventions

- Linux kernel C style, enforced by `.clang-format`: spaces only (indent
  width 8), 80-column limit, opening brace on its own line for functions but
  same line for control statements. Run `clang-format` before committing.
- C99 for C, C++17 for C++.
- `snake_case` for functions, types, and variables; `#pragma once` for headers.
- Public headers go in a module's `include/`, added with
  `target_include_directories(... PUBLIC include)` and consumed via angle
  brackets with a namespace prefix (`#include <led/hal.h>`), resolved by the
  self-symlink `include/<ns> -> .` in each include dir.

## Rules

1. Don't add any comments.
