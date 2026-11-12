# CHIP-8 Emulator

This repo contains my implementation of a C++ CHIP-8 emulator. Technically, it's
not an emulator at all, since CHIP-8 never existed as real hardware. It's just
another interpreter for CHIP-8 bytecode, but "emulator" sounds cooler, so I'm
sticking with it. Feed it a ROM and watch some 1970s pixels come to life.

## Getting Started

### Prerequisites

Before you begin, make sure you have the following tools installed on your
system:

- [C++20 compiler](https://en.cppreference.com/w/cpp/compiler_support/20)
- [CMake >= 3.30.0](https://cmake.org/)
- [Make](https://www.gnu.org/software/make/)
- [vcpkg](https://vcpkg.io/en/index.html)

### Build

First, clone this repo:

```sh
git clone https://github.com/michael-re/chip8.git
cd chip8/
```

Then build using the provided [**`makefile`**](makefile):

```sh
# Debug build
make debug

# Release build
make release

# Remove target and build artifacts
make clean
```

#### Build Output

Intermediate build files and compilation databases are placed under `build/`.
Compiled executables and libraries are placed under `target/`, organized by build
preset:

```txt
target/
├── debug/
│   ├── bin/
│   └── lib/
└── release/
    ├── bin/
    └── lib/
```

For example, the release executable is located at `target/release/bin/chip8`
(`chip8.exe` on Windows).

## Usage

```sh
chip8 --rom <ROM> [--scale <SCALE>] [--delay <DELAY>]
```

| Option              | Description                                                                          | Default |
|---------------------|--------------------------------------------------------------------------------------|---------|
| `-r, --rom ROM`     | Path to the CHIP-8 ROM file to run (required).                                       | -       |
| `-s, --scale SCALE` | Display scaling factor (1-100). Window size is `(64 x SCALE) x (32 x SCALE)`.        | `15`    |
| `-d, --delay DELAY` | CPU delay in milliseconds between cycles (1-100). Lower is faster, higher is slower. | `3`     |
| `-h, --help`        | Show the help message and exit.                                                      | -       |
| `-v, --version`     | Show version information and exit.                                                   | -       |

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
