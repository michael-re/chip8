#===============================================================================
#
# @file         CMakeLists.txt
# @author       michael-re
# @date         2026-11-08
# @copyright    copyright (c) 2026 michael-re, all rights reserved.
#
#===============================================================================
cmake_minimum_required(VERSION 3.30)
project(chip8 VERSION 0.0.0 LANGUAGES CXX)

# Configure compiler warning flags and disallow in-source builds
include(cmake/cxx_compiler_flags.cmake)
include(cmake/disallow_in_source.cmake)

# Collect source and header directories
set(chip8_inc_folder ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(chip8_src_folder ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Collect source and header files
file(GLOB_RECURSE chip8_hpp_files CONFIGURE_DEPENDS ${chip8_inc_folder}/chip8/*.hpp)
file(GLOB_RECURSE chip8_cpp_files CONFIGURE_DEPENDS ${chip8_src_folder}/chip8/*.cpp)

# Organize sources in ide
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
        ${chip8_hpp_files}
        ${chip8_cpp_files}
        ${chip8_src_folder}/main.cpp
)

# Create executable
add_executable(chip8
    ${chip8_hpp_files}
    ${chip8_cpp_files}
    ${chip8_src_folder}/main.cpp
)

# Add include directories
target_include_directories(chip8 PRIVATE ${chip8_inc_folder})
