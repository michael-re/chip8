///=============================================================================
///
/// @file         chip8.hpp
/// @author       michael-re
/// @date         2026-11-08
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================
#pragma once

#ifndef CHIP8_CHIP8_HPP
#define CHIP8_CHIP8_HPP

namespace chip8
{
    auto run(const char* rom, const int scale, const int delay) -> void;
} // namespace chip8

#endif /* CHIP8_CHIP8_HPP */
