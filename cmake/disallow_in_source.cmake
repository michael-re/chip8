# Make sure the user doesn't play dirty with symlinks
get_filename_component(src_dir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bin_dir "${CMAKE_BINARY_DIR}" REALPATH)

# Disallow in-source builds
if ("${src_dir}" STREQUAL "${bin_dir}")
    message(FATAL_ERROR"In-source builds not supported")
endif()
