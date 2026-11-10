///=============================================================================
///
/// @file         rng.cpp
/// @author       michael-re
/// @date         2026-11-09
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================

#include "chip8/rng.hpp"

template auto chip8::rng<std::int16_t >(std::int16_t,  std::int16_t ) -> std::int16_t;
template auto chip8::rng<std::int32_t >(std::int32_t,  std::int32_t ) -> std::int32_t;
template auto chip8::rng<std::int64_t >(std::int64_t,  std::int64_t ) -> std::int64_t;

template auto chip8::rng<std::uint16_t>(std::uint16_t, std::uint16_t) -> std::uint16_t;
template auto chip8::rng<std::uint32_t>(std::uint32_t, std::uint32_t) -> std::uint32_t;
template auto chip8::rng<std::uint64_t>(std::uint64_t, std::uint64_t) -> std::uint64_t;
