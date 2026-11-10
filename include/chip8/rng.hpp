///=============================================================================
///
/// @file         rng.hpp
/// @author       michael-re
/// @date         2026-11-09
/// @copyright    copyright (c) 2026 michael-re, all rights reserved.
///
///=============================================================================
#pragma once

#ifndef CHIP8_RNG_HPP
#define CHIP8_RNG_HPP

#include <chrono>
#include <concepts>
#include <cstdint>
#include <limits>
#include <random>

namespace chip8
{
    template<std::integral T>
    [[nodiscard]] auto rng(const T min = std::numeric_limits<T>::min(),
                           const T max = std::numeric_limits<T>::max()) -> T
    {
        using namespace std::chrono;

        static const auto now       = system_clock::now().time_since_epoch();
        static const auto time_seed = duration_cast<microseconds>(now).count();
        static const auto rand_seed = std::random_device{}();

        static auto engine = std::default_random_engine{
            static_cast<std::default_random_engine::result_type>(time_seed) +
            static_cast<std::default_random_engine::result_type>(rand_seed)
        };

        return std::uniform_int_distribution<T>{ min, max }(engine);
    }

    extern template auto rng<std::int16_t >(std::int16_t,  std::int16_t ) -> std::int16_t;
    extern template auto rng<std::int32_t >(std::int32_t,  std::int32_t ) -> std::int32_t;
    extern template auto rng<std::int64_t >(std::int64_t,  std::int64_t ) -> std::int64_t;

    extern template auto rng<std::uint16_t>(std::uint16_t, std::uint16_t) -> std::uint16_t;
    extern template auto rng<std::uint32_t>(std::uint32_t, std::uint32_t) -> std::uint32_t;
    extern template auto rng<std::uint64_t>(std::uint64_t, std::uint64_t) -> std::uint64_t;
} // namespace chip8

#endif /* CHIP8_RNG_HPP */
