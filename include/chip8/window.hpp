///=============================================================================
///
/// @file         window.hpp
/// @author       michael-re
/// @date         2026-11-09
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================
#pragma once

#ifndef CHIP8_WINDOW_HPP
#define CHIP8_WINDOW_HPP

class SDL_Window;
class SDL_Texture;
class SDL_Renderer;

namespace chip8
{
    class window
    {
    public:
        window(const char* title, int width, int height, int scale);
        ~window();

        auto update(const void* graphics, int pitch) -> void;
        auto handle_input(void* keyboard) -> bool;

    private:
        SDL_Window*         m_window;
        SDL_Texture*        m_texture;
        SDL_Renderer*       m_renderer;
    };
} // namespace chip8

#endif /* CHIP8_WINDOW_HPP */
