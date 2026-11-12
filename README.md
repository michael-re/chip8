# CHIP-8 Emulator

This repo contains my implementation of a C++ CHIP-8 emulator. Technically, it's
not an emulator at all, since CHIP-8 never existed as real hardware. It's just
another interpreter for CHIP-8 bytecode, but "emulator" sounds cooler, so I'm
sticking with it. Feed it a ROM and watch some 1970s pixels come to life.

## Getting Started

### Prerequisites

Before you begin, make sure you have the following tools installed on your
system:

| Tool | Notes |
|------|-------|
| [C++20 compiler](https://en.cppreference.com/w/cpp/compiler_support/20) | Recommended: [MSVC](https://visualstudio.microsoft.com/downloads/), [Clang](https://releases.llvm.org/download.html), or [GCC](https://gcc.gnu.org/). Tested on all three, so it should work on both Linux and Windows without issue. Not yet tested on macOS. |
| [CMake](https://cmake.org/) | Presets support is required, so 3.30 or later is recommended. |
| [vcpkg](https://github.com/microsoft/vcpkg) | Needed for the SDL2 and argparse dependencies. The presets locate it through the `VCPKG_ROOT` environment variable. |
| [Make](https://www.gnu.org/software/make/) | Not strictly required. You can run the CMake commands directly instead (see below). |

### Build

Start by cloning this repo:

```sh
git clone https://github.com/michael-re/chip8.git
cd chip8/
```

Then build using the provided makefile:

 - Debug build:
    ```sh
    make release
    ```

- Release build:
    ```sh
    make debug
    ```

Or call CMake directly:

1. Debug build:
    ```sh
    cmake -S . --preset release && cmake --build build/release
    ```

2. Release build:
    ```sh
    cmake -S . --preset debug && cmake --build build/debug
    ```

### Build Output

| Location                      | Contents                                                           |
|-------------------------------|--------------------------------------------------------------------|
| `build/<preset>/`             | CMake build files and intermediates                                |
| `build/compile_commands.json` | Copy of the compilation database for editor tooling (clangd, etc.) |
| `target/<preset>/bin/`        | The `chip8` executable (`chip8.exe` on Windows)                    |
| `target/<preset>/lib/`        | Library artifacts                                                  |

Where `<preset>` is either `debug` or `release`. For example, after `make release` the executable is at `./target/release/bin/chip8`.

## Usage

```sh
chip8 --rom <ROM> [--scale <SCALE>] [--delay <DELAY>]
```

| Option | Description | Default |
|--------|-------------|---------|
| `-r, --rom ROM` | Path to the CHIP-8 ROM file to run (required). ROMs load at `0x200` and cannot exceed 3584 bytes. | - |
| `-s, --scale SCALE` | Display scaling factor (1-100). Window size is `(64 x SCALE) x (32 x SCALE)`. | `15` |
| `-d, --delay DELAY` | CPU delay in milliseconds between cycles (1-100). Lower is faster, higher is slower. | `3` |
| `-h, --help` | Show the help message and exit. | - |
| `-v, --version` | Show version information and exit. | - |

### Examples

```sh
# Run a ROM with the default settings (960x480 window)
./target/release/bin/chip8 --rom roms/pong.ch8

# Larger window, with slower emulation
./target/release/bin/chip8 -r roms/tetris.ch8 -s 20 -d 5

# Smaller window, faster emulation
./target/release/bin/chip8 -r roms/invaders.ch8 -s 10 -d 1
```

If a game runs too fast or too slow, adjust `--delay`.

### Controls

The original CHIP-8 hex keypad is mapped to the left side of a QWERTY keyboard:

```
CHIP-8 keypad          Keyboard
+---+---+---+---+      +---+---+---+---+
| 1 | 2 | 3 | C |      | 1 | 2 | 3 | 4 |
+---+---+---+---+      +---+---+---+---+
| 4 | 5 | 6 | D |      | Q | W | E | R |
+---+---+---+---+  ->  +---+---+---+---+
| 7 | 8 | 9 | E |      | A | S | D | F |
+---+---+---+---+      +---+---+---+---+
| A | 0 | B | F |      | Z | X | C | V |
+---+---+---+---+      +---+---+---+---+
```

Press `Esc` or close the window to quit.
