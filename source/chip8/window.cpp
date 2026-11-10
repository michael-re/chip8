///=============================================================================
///
/// @file         window.cpp
/// @author       michael-re
/// @date         2026-11-09
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================

#include <array>
#include <cstdint>

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "chip8/window.hpp"

chip8::window::window(const char* title,
                      const int   width,
                      const int   height,
                      const int   scale)
    : m_window  (nullptr)
    , m_texture (nullptr)
    , m_renderer(nullptr)
{
    const auto format   = SDL_PIXELFORMAT_ABGR8888;
    const auto access   = SDL_TEXTUREACCESS_STREAMING;
    const auto position = SDL_WINDOWPOS_CENTERED;
    const auto x        = static_cast<int>(position);
    const auto y        = static_cast<int>(position);
    const auto w        = static_cast<int>(width);
    const auto h        = static_cast<int>(height);
    const auto sw       = static_cast<int>(scale * w);
    const auto sh       = static_cast<int>(scale * h);

    SDL_Init(SDL_INIT_VIDEO);

    m_window   = SDL_CreateWindow  (title, x, y, sw, sh, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    m_texture  = SDL_CreateTexture (m_renderer, format, access, w, h);
}

chip8::window::~window()
{
    SDL_DestroyTexture (m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow  (m_window);
    SDL_Quit();
}

auto chip8::window::update(const void* graphics, const int pitch) -> void
{
    SDL_UpdateTexture     (m_texture,  nullptr,   graphics, pitch  );
    SDL_RenderClear       (m_renderer                              );
    SDL_RenderCopy        (m_renderer, m_texture, nullptr,  nullptr);
    SDL_RenderPresent     (m_renderer                              );
}

auto chip8::window::handle_input(void* keypad) -> bool
{
    static constexpr auto keydown = 1;
    static constexpr auto keyup   = 0;
    static constexpr auto keymap  = std::to_array(
    {
        // keypad                  keymap
        // +---+---+---+---+       +---+---+---+---+
        // | 1 | 2 | 3 | C |       | 1 | 2 | 3 | 4 |
        // +---+---+---+---+       +---+---+---+---+
        // | 4 | 5 | 6 | D |       | Q | W | E | R |
        // +---+---+---+---+  ==>  +---+---+---+---+
        // | 7 | 8 | 9 | E |       | A | S | D | F |
        // +---+---+---+---+       +---+---+---+---+
        // | A | 0 | B | F |       | Z | X | C | V |
        // +---+---+---+---+       +---+---+---+---+
        SDLK_x, SDLK_1, SDLK_2, SDLK_3,
        SDLK_q, SDLK_w, SDLK_e, SDLK_a,
        SDLK_s, SDLK_d, SDLK_z, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v,
    });

    const auto handle_event = [&](const SDL_Event& event, const int state) -> auto
    {
        const auto key = event.key.keysym.sym;
        for (auto i = 0u; i < keymap.size(); ++i)
            if (key == keymap[i]) static_cast<std::uint8_t*>(keypad)[i] = state;
        return key;
    };

    // Process keyboard input
    for (auto e = SDL_Event(); SDL_PollEvent(&e);)
    {
        switch (e.type)
        {
            case SDL_QUIT:
            {
                // Terminate application
                return false;
            }
            case SDL_KEYDOWN:
            {
                if (handle_event(e, keydown) == SDLK_ESCAPE)
                    return false;
                break;
            }
            case SDL_KEYUP:
            {
                handle_event(e, keyup);
                break;
            }
            default:
            {
                // Ignore all other key events
                break;
            }
        }
    }

    return true;
}
