#include "khaos/Events/Event.h"
#include "khaos/Events/Events.h"
#include <catch2/catch_test_macros.hpp>

using KEvent::WindowResized;
using KhaosCore::Event;
using KhaosCore::EventDispatcher;

namespace {
    // A second event type, to check that the dispatcher tells types apart
    class OtherEvent : public Event {};
} // namespace

TEST_CASE("dispatcher calls the handler for the matching event type", "[events]") {
    WindowResized event(1280, 720);
    EventDispatcher dispatcher(event);

    int calls = 0;
    int width = 0;
    int height = 0;
    dispatcher.Dispatch<WindowResized>([&](WindowResized& resized) {
        ++calls;
        width = resized.width();
        height = resized.height();
        return true;
    });

    REQUIRE(calls == 1);
    REQUIRE(width == 1280);
    REQUIRE(height == 720);
    REQUIRE(event.isHandled);
}

TEST_CASE("dispatcher skips handlers for other event types", "[events]") {
    OtherEvent event;
    EventDispatcher dispatcher(event);

    bool called = false;
    dispatcher.Dispatch<WindowResized>([&](WindowResized&) {
        called = true;
        return true;
    });

    REQUIRE_FALSE(called);
    REQUIRE_FALSE(event.isHandled);
}

TEST_CASE("a handled event is not passed to later handlers", "[events]") {
    WindowResized event(800, 600);
    EventDispatcher dispatcher(event);

    int first = 0;
    int second = 0;
    dispatcher.Dispatch<WindowResized>([&](WindowResized&) {
        ++first;
        return true;
    });
    dispatcher.Dispatch<WindowResized>([&](WindowResized&) {
        ++second;
        return true;
    });

    REQUIRE(first == 1);
    REQUIRE(second == 0);
}

TEST_CASE("an event left unhandled reaches the next handler", "[events]") {
    WindowResized event(800, 600);
    EventDispatcher dispatcher(event);

    int first = 0;
    int second = 0;
    dispatcher.Dispatch<WindowResized>([&](WindowResized&) {
        ++first;
        return false;
    });
    REQUIRE_FALSE(event.isHandled);

    dispatcher.Dispatch<WindowResized>([&](WindowResized&) {
        ++second;
        return true;
    });

    REQUIRE(first == 1);
    REQUIRE(second == 1);
    REQUIRE(event.isHandled);
}
