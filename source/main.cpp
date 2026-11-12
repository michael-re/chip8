///=============================================================================
///
/// @file         main.cpp
/// @author       michael-re
/// @date         2026-11-08
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================

#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <string>

#include <argparse/argparse.hpp>

#include "chip8/chip8.hpp"

constexpr auto k_version = "1.0.0";
constexpr auto k_help    = R"(A simple CHIP-8 emulator

Usage:
  chip8 [--help] [--version] --rom ROM [--scale SCALE] [--delay DELAY]

Arguments:
  -h, --help        : Displays this help message and exits.
  -v, --version     : Displays version information and exits.
  -r, --rom ROM     : Path to the CHIP-8 ROM file to execute. The ROM
                      is loaded into memory starting at address 0x200.
                      Since CHIP-8 has 4 KiB (4096 bytes) of memory,
                      ROM files cannot exceed 3584 bytes (3.5 KiB).
  -s, --scale SCALE : Display scaling factor (1-100). The CHIP-8
                      display has a resolution of 64x32 pixels.
                      Each pixel is scaled by this factor, producing
                      a window resolution of (64 x SCALE) x (32 x SCALE)
                      pixels. For example, a scale of 15 produces a
                      960x480 window. (default: 15)
  -d, --delay DELAY : CPU execution delay (1-100). Controls the pacing
                      of CPU instruction execution. Smaller values
                      allow instructions to execute more frequently,
                      resulting in faster emulation. Larger values
                      introduce more delay between execution steps,
                      resulting in slower emulation. (default: 3)
)";

auto main(const int argc, const char** argv) -> int
{
    auto program = argparse::ArgumentParser(
        "chip8", k_version, argparse::default_arguments::none
    );

    // Help
    program
        .add_argument("-h", "--help")
        .action([&](const std::string&) {
            std::cout << k_help << "\n";
            std::exit(EXIT_SUCCESS);
        })
        .default_value(false)
        .help("Displays this help message and exits.")
        .implicit_value(true)
        .nargs(0);

    // Version
    program
        .add_argument("-v", "--version")
        .action([&](const std::string&) {
            std::cout << "chip8 " << k_version << "\n";
            std::exit(EXIT_SUCCESS);
        })
        .default_value(false)
        .help("Displays version information and exits.")
        .implicit_value(true)
        .nargs(0);

    // ROM
    program
        .add_argument("-r", "--rom")
        .help("Path to the CHIP-8 ROM file.")
        .metavar("ROM")
        .required();

    // Argument validation
    const auto check_arg_range = [](
        const std::string_view name,
        const std::string&     arg,
        const std::int64_t     min = 1,
        const std::int64_t     max = 100
    )
    {
        const auto value = std::stoll(arg);
        if (value < min || value > max)
        {
            throw std::runtime_error(
                std::format(
                    "'{}' expects a value between {} and {}, but received {}.",
                    name,
                    min,
                    max,
                    value
                )
            );
        }
    };

    // Display scaling
    program
        .add_argument("-s", "--scale")
        .help("Display scaling factor (1-100).")
        .metavar("SCALE")
        .scan<'i', int>()
        .default_value(15)
        .implicit_value(15)
        .nargs(1)
        .action([&](const std::string& arg) { check_arg_range("--delay", arg); });

    // Execution delay
    program
        .add_argument("-d", "--delay")
        .help("CPU execution delay (1-100).")
        .metavar("DELAY")
        .scan<'i', int>()
        .default_value(3)
        .implicit_value(3)
        .nargs(1)
        .action([&](const std::string& arg) { check_arg_range("--delay", arg); });

    // Parse args
    try
    {
        program.parse_args(argc, argv);
        static_cast<void>(program.get<std::string>("--rom"));
        static_cast<void>(program.get<int>("--scale"));
        static_cast<void>(program.get<int>("--delay"));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error:\n    " << e.what() << "\n\n";
        std::cerr << k_help << "\n";
        return EXIT_FAILURE;
    }

    // Run emulator
    try
    {
        chip8::run(
            program.get<std::string>("--rom").c_str(),
            program.get<int>("--scale"),
            program.get<int>("--delay")
        );
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
