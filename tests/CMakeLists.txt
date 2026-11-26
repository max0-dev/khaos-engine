find_package(Catch2 3 CONFIG REQUIRED)

add_executable(khaos_tests
    src/LogTests.cpp
)
target_link_libraries(khaos_tests PRIVATE
    khaos::core
    khaos::options
    Catch2::Catch2WithMain
)
khaos_set_output_dirs(khaos_tests)

include(Catch)
catch_discover_tests(khaos_tests DISCOVERY_MODE PRE_TEST)
