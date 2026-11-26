#include "khaos/core/Log.h"

#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace khaos {

std::shared_ptr<spdlog::logger> Log::s_core;

void Log::init(std::string_view appName) {
    if (s_core) {
        return;
    }
    s_core = spdlog::stdout_color_mt(std::string(appName));
    s_core->set_pattern("%^[%T] %n: %v%$");
#ifdef KHAOS_DEBUG
    s_core->set_level(spdlog::level::trace);
#else
    s_core->set_level(spdlog::level::info);
#endif
}

void Log::shutdown() {
    if (s_core) {
        spdlog::drop(s_core->name());
        s_core.reset();
    }
}

std::shared_ptr<spdlog::logger>& Log::core() {
    if (!s_core) {
        init();
    }
    return s_core;
}

bool Log::isInitialized() noexcept { return s_core != nullptr; }

} // namespace khaos
