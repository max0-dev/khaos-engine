#pragma once

#include <memory>
#include <string_view>

#include <spdlog/spdlog.h>

namespace khaos {

// Thin logging facade over spdlog. Placeholder - reshape freely as the core architecture evolves.
class Log {
public:
    static void init(std::string_view appName = "khaos");
    static void shutdown();

    [[nodiscard]] static std::shared_ptr<spdlog::logger>& core();
    [[nodiscard]] static bool isInitialized() noexcept;

private:
    static std::shared_ptr<spdlog::logger> s_core;
};

[[nodiscard]] constexpr std::string_view version() noexcept { return KHAOS_VERSION_STRING; }

} // namespace khaos

#define KHAOS_TRACE(...) ::khaos::Log::core()->trace(__VA_ARGS__)
#define KHAOS_INFO(...)  ::khaos::Log::core()->info(__VA_ARGS__)
#define KHAOS_WARN(...)  ::khaos::Log::core()->warn(__VA_ARGS__)
#define KHAOS_ERROR(...) ::khaos::Log::core()->error(__VA_ARGS__)
