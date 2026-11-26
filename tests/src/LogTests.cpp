#include <catch2/catch_test_macros.hpp>

#include "khaos/core/Log.h"

TEST_CASE("Log initializes and shuts down", "[core][log]") {
    khaos::Log::shutdown();
    REQUIRE_FALSE(khaos::Log::isInitialized());

    khaos::Log::init("test");
    REQUIRE(khaos::Log::isInitialized());
    REQUIRE(khaos::Log::core()->name() == "test");

    SECTION("init is idempotent") {
        khaos::Log::init("other");
        REQUIRE(khaos::Log::core()->name() == "test");
    }

    khaos::Log::shutdown();
    REQUIRE_FALSE(khaos::Log::isInitialized());
}

TEST_CASE("Version string is set", "[core]") {
    REQUIRE_FALSE(khaos::version().empty());
}
