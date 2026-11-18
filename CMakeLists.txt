cmake_minimum_required(VERSION 3.26)

# vcpkg comes from the submodule unless a toolchain was passed in (-DCMAKE_TOOLCHAIN_FILE=...).
# It bootstraps itself and installs vcpkg.json's dependencies on the first configure.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(khaos VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build the tests" ON)
option(ENABLE_SANITIZERS "Build with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

include(cmake/ProjectOptions.cmake)
include(cmake/Shaders.cmake)

add_subdirectory(external/khaos-tui)
add_subdirectory(core)
add_subdirectory(editor)

# Shaders in res/ are checked on every build, then res/ is copied next to the editor
# (out/<preset>/res) so it finds files with relative paths like "res/core/object_lit.vert".
# Only changed files are copied, and only after the check, so a broken shader never lands in out/.
# The editor depends on both, so building only the editor target (cmake-tools' build/run) runs them.
project_add_shader_check(shaders ${CMAKE_CURRENT_SOURCE_DIR}/res)

add_custom_target(copy_res ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:editor>/res
    COMMENT "Copying res to the output folder"
    VERBATIM
)
add_dependencies(copy_res shaders)
add_dependencies(editor copy_res)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
