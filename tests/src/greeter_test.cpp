#include <Greeter.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("framework runs", "[smoke]") { REQUIRE(1 == 1); }

TEST_CASE("Greeter greets by name", "[greeter]") {
    const Core::Greeter greeter("Ada");
    CHECK(greeter.GetName() == "Ada");
    CHECK(greeter.Greet() == "Hello, Ada!");
}
