///=============================================================================
///
/// @file         emulator.cpp
/// @author       michael-re
/// @date         2026-11-09
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================

#include <algorithm>
#include <bit>
#include <cstdint>
#include <format>
#include <fstream>
#include <stdexcept>

#include "chip8/emulator.hpp"
#include "chip8/font.hpp"
#include "chip8/rng.hpp"

#define vf        (0xfu)
#define vx        ((m_opcode & 0x0f00u) >> 8u)
#define vy        ((m_opcode & 0x00f0u) >> 4u)
#define addr_nnn  (m_opcode & 0x0fffu)
#define byte_kk   (m_opcode & 0x00ffu)
#define reg_vf    (m_registers[vf])
#define reg_vx    (m_registers[vx])
#define reg_vy    (m_registers[vy])

auto chip8::emulator::init() -> void
{
    m_pc          = k_address_start;
    m_ir          = 0x000u;
    m_sp          = 0x000u;
    m_dt          = 0x000u;
    m_st          = 0x000u;
    m_opcode      = 0x000u;
    m_stack   .fill(0x000u);
    m_memory  .fill(0x000u);
    m_keypad  .fill(0x000u);
    m_graphics.fill(0x000u);

    // Load fontset
    std::copy(
        font::k_sprites.begin(),
        font::k_sprites.begin() + font::k_sprites.size(),
              m_memory .begin() + font::k_start_address
    );
}

auto chip8::emulator::load(const std::string &rom) -> void
{
    // Reset emulator state before loading new ROM file
    init();

    // Open file
    auto file = std::ifstream(rom, std::ifstream::binary | std::ifstream::ate);
    if (!file.is_open())
    {
        throw std::runtime_error(
            std::format("failed to open ROM file '{}'", rom)
        );
    }

    // Ensure it's not too large to fit into memory
    const auto size = file.tellg();
    if (size >= (m_memory.size() - k_address_start))
    {
        throw std::runtime_error(
            std::format(
                "ROM file '{}' is too large: {} bytes (maximum {} bytes)",
                rom,
                static_cast<std::size_t>(size),
                static_cast<std::size_t>(m_memory.size() - k_address_start)
            )
        );
    }

    // Load into memory
    file.seekg(0x0, std::ifstream::beg);
    file.read(std::bit_cast<char*>(&m_memory[k_address_start]), size);
    file.close();
}

auto chip8::emulator::emulate() -> void
{
    // Fetch
    m_opcode = (m_memory[m_pc] << 8u) | m_memory[m_pc + 1];
    m_pc += 2;

    // Decode and Execute
    execute();

    // Decrement timers if they are set
    if (m_dt > 0) m_dt--;
    if (m_sp > 0) m_st--;
}

auto chip8::emulator::execute() -> void
{
    using op_handler = void (emulator::*)();
    using    handler = void (*)(emulator&);

    static constexpr auto table0 = []
    {
        auto table = std::array<op_handler, 0x10>{};
        for (auto& e : table) e = &emulator::op_null;
        table[0x0] = &emulator::op_00e0;
        table[0xe] = &emulator::op_00ee;
        return table;
    }();

    static constexpr auto table8 = []
    {
        auto table = std::array<op_handler, 0x10>{};
        for (auto& e : table) e = &emulator::op_null;
        table[0x0] = &emulator::op_8xy0;
        table[0x1] = &emulator::op_8xy1;
        table[0x2] = &emulator::op_8xy2;
        table[0x3] = &emulator::op_8xy3;
        table[0x4] = &emulator::op_8xy4;
        table[0x5] = &emulator::op_8xy5;
        table[0x6] = &emulator::op_8xy6;
        table[0x7] = &emulator::op_8xy7;
        table[0xe] = &emulator::op_8xye;
        return table;
    }();

    static constexpr auto tablee = []
    {
        auto table = std::array<op_handler, 0x10>{};
        for (auto& e : table) e = &emulator::op_null;
        table[0x1] = &emulator::op_exa1;
        table[0xe] = &emulator::op_ex9e;
        return table;
    }();

    static constexpr auto tablef = []
    {
        std::array<op_handler, 0x80> table{};
        for (auto& e : table) e = &emulator::op_null;
        table[0x07] = &emulator::op_fx07;
        table[0x0a] = &emulator::op_fx0a;
        table[0x15] = &emulator::op_fx15;
        table[0x18] = &emulator::op_fx18;
        table[0x1e] = &emulator::op_fx1e;
        table[0x29] = &emulator::op_fx29;
        table[0x33] = &emulator::op_fx33;
        table[0x55] = &emulator::op_fx55;
        table[0x65] = &emulator::op_fx65;
        return table;
    }();

    static constexpr auto table = []
    {
        auto table = std::array<handler, 0x10>{};
        table[0x0] = [](emulator& self) { (self.*table0[self.m_opcode & 0x000fu])(); };
        table[0x1] = [](emulator& self) { (self.*&emulator::op_1nnn)(); };
        table[0x2] = [](emulator& self) { (self.*&emulator::op_2nnn)(); };
        table[0x3] = [](emulator& self) { (self.*&emulator::op_3xkk)(); };
        table[0x4] = [](emulator& self) { (self.*&emulator::op_4xkk)(); };
        table[0x5] = [](emulator& self) { (self.*&emulator::op_5xy0)(); };
        table[0x6] = [](emulator& self) { (self.*&emulator::op_6xkk)(); };
        table[0x7] = [](emulator& self) { (self.*&emulator::op_7xkk)(); };
        table[0x8] = [](emulator& self) { (self.*table8[self.m_opcode & 0x000fu])(); };
        table[0x9] = [](emulator& self) { (self.*&emulator::op_9xy0)(); };
        table[0xa] = [](emulator& self) { (self.*&emulator::op_annn)(); };
        table[0xb] = [](emulator& self) { (self.*&emulator::op_bnnn)(); };
        table[0xc] = [](emulator& self) { (self.*&emulator::op_cxkk)(); };
        table[0xd] = [](emulator& self) { (self.*&emulator::op_dxyn)(); };
        table[0xe] = [](emulator& self) { (self.*tablee[self.m_opcode & 0x000fu])(); };
        table[0xf] = [](emulator& self)
        {
            const auto idx = self.m_opcode & 0x00ffu;
            (self.*(idx < tablef.size() ? tablef[idx] : &emulator::op_null))();
        };

        return table;
    }();

    // Decode instruction and execute
    table[(m_opcode & 0xf000u) >> 12u](*this);
}

auto chip8::emulator::op_null() -> void
{
    // do nothing
}

auto chip8::emulator::op_00e0() -> void
{
    m_graphics.fill(0x00);
}

auto chip8::emulator::op_00ee() -> void
{
    // Top of stack has address of one instruction past the one that called the
    // subroutine.
    m_pc = m_stack[--m_sp];
}

auto chip8::emulator::op_1nnn() -> void
{
    // Jump does not remember its origin, no stack interaction required.
    m_pc = addr_nnn;
}

auto chip8::emulator::op_2nnn() -> void
{
    m_stack[m_sp++] = m_pc; // Save program counter on stack in new frame
    m_pc = addr_nnn;        // Call routine at location nnn given opcode 2nnn
}

auto chip8::emulator::op_3xkk() -> void
{
    if (reg_vx == byte_kk) m_pc += 2;
}

auto chip8::emulator::op_4xkk() -> void
{
    if (reg_vx != byte_kk) m_pc += 2;
}

auto chip8::emulator::op_5xy0() -> void
{
    if (reg_vx == reg_vy) m_pc += 2;
}

auto chip8::emulator::op_6xkk() -> void
{
    reg_vx = byte_kk;
}

auto chip8::emulator::op_7xkk() -> void
{
    reg_vx += byte_kk;
}

auto chip8::emulator::op_8xy0() -> void
{
    reg_vx = reg_vy;
}

auto chip8::emulator::op_8xy1() -> void
{
    reg_vx |= reg_vy;
}

auto chip8::emulator::op_8xy2() -> void
{
    reg_vx &= reg_vy;
}

auto chip8::emulator::op_8xy3() -> void
{
    reg_vx ^= reg_vy;
}

auto chip8::emulator::op_8xy4() -> void
{
    const auto sum = uint16_t(reg_vx + reg_vy);
    reg_vf = sum > 0xffu ? 1u : 0u;
    reg_vx = sum & 0xffu; // Keep lowest 8 bits of the result and store in vx
}

auto chip8::emulator::op_8xy5() -> void
{
    reg_vf  = reg_vx > reg_vy ? 1u : 0u;
    reg_vx -= reg_vy;
}

auto chip8::emulator::op_8xy6() -> void
{
    reg_vf   = (reg_vx & 0x1u); // Save least significant bit of vx in vf
    reg_vx >>= 1;               // Divide vx by 2
}

auto chip8::emulator::op_8xy7() -> void
{
    reg_vf = reg_vy > reg_vx ? 1u : 0u;
    reg_vx = reg_vy - reg_vx;
}

auto chip8::emulator::op_8xye() -> void
{
    reg_vf   = (reg_vx & 0x80u) >> 7u; // Save most significant bit of vx in vf
    reg_vx <<= 1;                      // Multiply vx by two
}

auto chip8::emulator::op_9xy0() -> void
{
    if (reg_vx != reg_vy) m_pc += 2;
}

auto chip8::emulator::op_annn() -> void
{
    m_ir = addr_nnn;
}

auto chip8::emulator::op_bnnn() -> void
{
    m_pc = m_registers[0] + addr_nnn;
}

auto chip8::emulator::op_cxkk() -> void
{
    reg_vx = rng<std::uint64_t>(0u, 255u) & byte_kk;
}

auto chip8::emulator::op_dxyn() -> void
{
    // Conflict about sprite behavior when it reaches the screens edge. Some
    // prefer the sprite being clipped and others the sprite wrapping. I prefer
    // the sprites wrapping when they reach the screen edge.
    const auto x      = std::uint8_t (reg_vx   % k_video_width );
    const auto y      = std::uint8_t (reg_vy   % k_video_height);
    const auto height = std::uint8_t (m_opcode & vf            );
    const auto on     = std::uint32_t(0xffffffff               );

    reg_vf = 0u;

    for (auto row = std::uint32_t(0u); row < height; row++)
    {
        // Get sprite from memory
        const auto nth_sprite_byte = std::uint8_t(m_memory[m_ir + row]);

        // Iterating over each pixel in current row
        for (auto pixel = std::uint32_t(0u); pixel < 8u; pixel++)
        {
            auto screen_pixel = &m_graphics[
                (y +  static_cast<std::size_t>(row)) * k_video_width
                   + (static_cast<std::size_t>(x)    + pixel)
            ];

            // Sprite pixel is on
            if (nth_sprite_byte & (0x80u >> pixel))
            {
                if (*screen_pixel == on)
                {
                    // Collision at current pixel - screen and sprite pixel on
                    reg_vf = 1u;
                }

                // xor screen pixel with 0xffffffff to xor sprite pixel
                *screen_pixel ^= on;
            }
        }
    }
}

auto chip8::emulator::op_ex9e() -> void
{
    if (m_keypad[reg_vx]) m_pc += 2;
}

auto chip8::emulator::op_exa1() -> void
{
    if (!m_keypad[reg_vx]) m_pc += 2;
}

auto chip8::emulator::op_fx07() -> void
{
    reg_vx = m_dt;
}

auto chip8::emulator::op_fx0a() -> void
{
    // If keypress not detected, decrement program counter since it's
    // incremented in each execution loop.
    m_keypad[0x00] ? reg_vx = 0x00 :
    m_keypad[0x01] ? reg_vx = 0x01 :
    m_keypad[0x02] ? reg_vx = 0x02 :
    m_keypad[0x03] ? reg_vx = 0x03 :
    m_keypad[0x04] ? reg_vx = 0x04 :
    m_keypad[0x05] ? reg_vx = 0x05 :
    m_keypad[0x06] ? reg_vx = 0x06 :
    m_keypad[0x07] ? reg_vx = 0x07 :
    m_keypad[0x08] ? reg_vx = 0x08 :
    m_keypad[0x09] ? reg_vx = 0x09 :
    m_keypad[0x0a] ? reg_vx = 0x0a :
    m_keypad[0x0b] ? reg_vx = 0x0b :
    m_keypad[0x0c] ? reg_vx = 0x0c :
    m_keypad[0x0d] ? reg_vx = 0x0d :
    m_keypad[0x0e] ? reg_vx = 0x0e :
    m_keypad[0x0f] ? reg_vx = 0x0f :
    m_pc -= 2;
}

auto chip8::emulator::op_fx15() -> void
{
    m_dt = reg_vx;
}

auto chip8::emulator::op_fx18() -> void
{
    m_st = reg_vx;
}

auto chip8::emulator::op_fx1e() -> void
{
    m_ir += reg_vx;
}

auto chip8::emulator::op_fx29() -> void
{
    // Since the font character is located at 0x50 and is five bytes, the value
    // of i is set to the location for the hexadecimal sprite corresponding to
    // the value of vx by taking an offset from the start address.
    m_ir = font::k_start_address + (font::k_sprite_offset * reg_vx);
}

auto chip8::emulator::op_fx33() -> void
{
    // The emulator takes the decimal value of vx, and places the hundreds
    // digit in memory at location in i, the tens digit at location i + 1,
    // and the ones digit at location i + 2.
    const auto value = std::uint8_t(reg_vx);
    m_memory[static_cast<std::size_t>(m_ir) + 2] = (value / 1  ) % 10;
    m_memory[static_cast<std::size_t>(m_ir) + 1] = (value / 10 ) % 10;
    m_memory[static_cast<std::size_t>(m_ir) + 0] = (value / 100) % 10;
}

auto chip8::emulator::op_fx55() -> void
{
    for (auto i = std::uint8_t(0); i <= vx; i++)
        m_memory[static_cast<std::size_t>(m_ir) + i] = m_registers[i];
}

auto chip8::emulator::op_fx65() -> void
{
    for (auto i = std::uint8_t(0); i < vx; i++)
        m_registers[i] = m_memory[static_cast<std::size_t>(m_ir) + i];
}
