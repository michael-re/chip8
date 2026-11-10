///=============================================================================
///
/// @file         chip8.cpp
/// @author       michael-re
/// @date         2026-11-08
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================

#include <chrono>
#include <memory>

#include "chip8/chip8.hpp"
#include "chip8/window.hpp"
#include "chip8/emulator.hpp"

auto chip8::run(const char* rom, const int scale, const int delay) -> void
{
    using clock    = std::chrono::high_resolution_clock;
    using period   = std::chrono::milliseconds::period;
    using duration = std::chrono::duration<double, period>;

    const auto width  = emulator::k_video_width;
    const auto height = emulator::k_video_height;
    const auto pitch  = sizeof(emulator::m_graphics[0]) * width;
    const auto title  = "CHIP-8 Emulator";

    auto emulator = std::make_unique<class emulator>();
    emulator->init();
    emulator->load(rom);

    auto window = std::make_unique<class window>(title, width, height, scale);
    auto saved  = clock::now();

    while (window->handle_input(emulator->m_keypad.data()))
    {
        // Get elapsed time
        const auto time  = clock::now();
        const auto delta = duration(time - saved).count();

        // Usage delta time to control framerate
        if (delta > delay)
        {
            // Save time
            saved = time;

            // Run new emulation cycle
            emulator->emulate();

            // Update screen
            window->update(emulator->m_graphics.data(), pitch);
        }
    }
}
