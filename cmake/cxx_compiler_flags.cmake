# See: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#msvc
# cspell: disable
set(_cxx_msvc_flags
    /external:anglebrackets         # treat files included with <> as external
    /external:W0                    # turn off warnings from all external header files
    /Zc:preprocessor                # enables a token-based preprocessor that conforms to C99 and C++11 and later standards
    /W4                             # all reasonable warnings
    /w14242                         # 'identifier': conversion from 'type1' to 'type1', possible loss of data
    /w14254                         # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
    /w14263                         # 'function': member function does not override any base class virtual member function
    /w14265                         # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
    /w14287                         # 'operator': unsigned/negative constant mismatch
    /we4289                         # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
    /w14296                         # 'operator': expression is always 'boolean_value'
    /w14311                         # 'variable': pointer truncation from 'type1' to 'type2'
    /w14545                         # expression before comma evaluates to a function which is missing an argument list
    /w14546                         # function call before comma missing argument list
    /w14547                         # 'operator': operator before comma has no effect; expected operator with side-effect
    /w14549                         # 'operator': operator before comma has no effect; did you intend 'operator'?
    /w14555                         # expression has no effect; expected expression with side- effect
    /w14619                         # pragma warning: there is no warning number 'number'
    /w14640                         # Enable warning on thread un-safe static member initialization
    /w14826                         # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
    /w14905                         # wide string literal cast to 'LPSTR'
    /w14906                         # string literal cast to 'LPWSTR'
    /w14928                         # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    /permissive-                    # standards conformance mode for MSVC compiler.
    /WX
)
# cspell: enable

# See: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#gcc--clang
# cspell: disable
set(_cxx_clang_flags
    -Wall
    -Wextra                         # reasonable and standard
    -Wnon-virtual-dtor              # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
    -Wold-style-cast                # warn for c-style casts
    -Wcast-align                    # warn for potential performance problem casts
    -Wunused                        # warn on anything being unused
    -Woverloaded-virtual            # warn if you overload (not override) a virtual function
    -Wpedantic                      # warn if non-standard C++ is used
    -Wconversion                    # warn on type conversions that may lose data
    -Wsign-conversion               # warn on sign conversions
    -Wnull-dereference              # warn if a null dereference is detected
    -Wdouble-promotion              # warn if float is implicit promoted to double
    -Wformat=2                      # warn on security issues around functions that format output (ie printf)
    -Wimplicit-fallthrough          # warn on statements that fallthrough without an explicit annotation
    -Werror
)
# cspell: enable

# See: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#gcc--clang
# cspell: disable
set(_cxx_gcc_flags
    ${_cxx_clang_flags}             # add all clang compile options
    -Wmisleading-indentation        # warn if indentation implies blocks where blocks do not exist
    -Wduplicated-cond               # warn if if / else chain has duplicated conditions
    -Wduplicated-branches           # warn if if / else branches have duplicated code
    -Wlogical-op                    # warn about logical operations being used where bitwise were probably wanted
    -Wuseless-cast                  # warn if you perform a cast to the same type
)
# cspell: enable

# See https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
set(cxx_compiler_flags "")
if(MSVC)
    list(APPEND cxx_compiler_flags ${_cxx_msvc_flags})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND cxx_compiler_flags ${_cxx_clang_flags})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND cxx_compiler_flags ${_cxx_gcc_flags})
else()
    message(STATUS "[WARNING] Unrecognized C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "[WARNING] Skipping compiler-specific warning flags configuration for ${CMAKE_CXX_COMPILER_ID}")
endif()
