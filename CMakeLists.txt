cmake_minimum_required(VERSION 3.25)

# vcpkg (git submodule, manifest mode) — must be set before project()
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(khaos VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(KHAOS_BUILD_EDITOR "Build the Khaos editor" ON)
option(KHAOS_BUILD_TESTS  "Build the Khaos tests"  ON)

# Shared warnings / flags for every khaos target
add_library(khaos_options INTERFACE)
add_library(khaos::options ALIAS khaos_options)
if(MSVC)
    target_compile_options(khaos_options INTERFACE /W4 /permissive- /utf-8 /Zc:__cplusplus)
    target_compile_definitions(khaos_options INTERFACE NOMINMAX WIN32_LEAN_AND_MEAN)
else()
    target_compile_options(khaos_options INTERFACE -Wall -Wextra -Wpedantic)
endif()

function(khaos_set_output_dirs target)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/$<CONFIG>
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/$<CONFIG>
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/$<CONFIG>
        PDB_OUTPUT_DIRECTORY     ${CMAKE_SOURCE_DIR}/out/$<CONFIG>
    )
endfunction()

add_subdirectory(core)

if(KHAOS_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if(KHAOS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
