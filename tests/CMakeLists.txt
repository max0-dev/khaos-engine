# tests: Catch2 v3 tests against core. Every TEST_CASE becomes its own ctest entry.

find_package(Catch2 3 CONFIG REQUIRED)

file(GLOB_RECURSE TEST_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(tests ${TEST_SOURCES})
project_setup_target(tests)

target_link_libraries(tests PRIVATE
    core
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(tests)
