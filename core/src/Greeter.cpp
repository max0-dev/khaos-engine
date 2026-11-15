#include "Greeter.h"

#include <fmt/format.h>

#include <string>
#include <utility>

namespace Core {

Greeter::Greeter(std::string name) : mName(std::move(name)) {}

std::string Greeter::Greet() const { return fmt::format("Hello, {}!", mName); }

} // namespace Core
