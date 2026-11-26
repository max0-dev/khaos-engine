find_package(spdlog CONFIG REQUIRED)

add_library(khaos_core STATIC
    src/Log.cpp
)
add_library(khaos::core ALIAS khaos_core)

target_include_directories(khaos_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(khaos_core
    PUBLIC  spdlog::spdlog
    PRIVATE khaos::options
)
target_compile_definitions(khaos_core PUBLIC
    KHAOS_VERSION_STRING="${PROJECT_VERSION}"
    $<$<CONFIG:Debug>:KHAOS_DEBUG>
)
khaos_set_output_dirs(khaos_core)
