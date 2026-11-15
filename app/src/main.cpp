#include <Greeter.h>

#include <fmt/base.h>

int main(int argc, char** argv) {
    const Core::Greeter greeter(argc > 1 ? argv[1] : "world");
    fmt::println("{}", greeter.Greet());
    return 0;
}
