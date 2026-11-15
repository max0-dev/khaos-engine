#include "khaos/App/App.h"
#include <KHR/khrplatform.h>
#include <fmt/base.h>

int main() {
    fmt::println("Hello World");
    KhaosCore::App app;
    app.Run();
}
