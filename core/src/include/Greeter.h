#pragma once

#include <string>

namespace Core {

// Example class: replace it with your own code.
class Greeter {
  public:
    explicit Greeter(std::string name);

    [[nodiscard]] std::string Greet() const;
    [[nodiscard]] const std::string& GetName() const { return mName; }

  private:
    std::string mName;
};

} // namespace Core
