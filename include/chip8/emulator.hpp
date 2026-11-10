///=============================================================================
///
/// @file         emulator.hpp
/// @author       michael-re
/// @date         2026-11-09
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================
#pragma once

#ifndef CHIP8_EMULATOR_HPP
#define CHIP8_EMULATOR_HPP

#include <array>
#include <cstdint>
#include <string>

namespace chip8
{
    class emulator
    {
    public:
        static inline constexpr auto k_video_width   = 0x040u;
        static inline constexpr auto k_video_height  = 0x020u;
        static inline constexpr auto k_address_vf    = 0x00fu;
        static inline constexpr auto k_address_start = 0x200u;

    public:
        std::array<std::uint8_t,    16u> m_keypad    = {};
        std::array<std::uint32_t, 2048u> m_graphics  = {};
        std::array<std::uint8_t,  4096u> m_memory    = {};
        std::array<std::uint16_t,   16u> m_stack     = {};
        std::array<std::uint8_t,    16u> m_registers = {};
        std::uint16_t                    m_ir        = 0x0u;
        std::uint16_t                    m_pc        = 0x0u;
        std::uint8_t                     m_sp        = 0x0u;
        std::uint8_t                     m_dt        = 0x0u;
        std::uint8_t                     m_st        = 0x0u;
        std::uint16_t                    m_opcode    = 0x0u;

    public:
        auto init()                       -> void;
        auto load(const std::string& rom) -> void;

    public:
        auto emulate() -> void;
        auto execute() -> void;
        auto op_null() -> void;
        auto op_00e0() -> void;
        auto op_00ee() -> void;
        auto op_1nnn() -> void;
        auto op_2nnn() -> void;
        auto op_3xkk() -> void;
        auto op_4xkk() -> void;
        auto op_5xy0() -> void;
        auto op_6xkk() -> void;
        auto op_7xkk() -> void;
        auto op_8xy0() -> void;
        auto op_8xy1() -> void;
        auto op_8xy2() -> void;
        auto op_8xy3() -> void;
        auto op_8xy4() -> void;
        auto op_8xy5() -> void;
        auto op_8xy6() -> void;
        auto op_8xy7() -> void;
        auto op_8xye() -> void;
        auto op_9xy0() -> void;
        auto op_annn() -> void;
        auto op_bnnn() -> void;
        auto op_cxkk() -> void;
        auto op_dxyn() -> void;
        auto op_ex9e() -> void;
        auto op_exa1() -> void;
        auto op_fx07() -> void;
        auto op_fx0a() -> void;
        auto op_fx15() -> void;
        auto op_fx18() -> void;
        auto op_fx1e() -> void;
        auto op_fx29() -> void;
        auto op_fx33() -> void;
        auto op_fx55() -> void;
        auto op_fx65() -> void;
    };
} // namespace chip8

#endif /* CHIP8_EMULATOR_HPP */
