#include "KCoreLog.h"
#include "khaos/KLog.h"
#include <format>
#include <spdlog/spdlog.h>
#include <string>
using namespace KhaosCore;

void KLog::Info(const char* msg) {
#ifdef KHAOS_DEBUG
    KLogger::AddMessageToBuffer(
        LogMessage{.msg = msg, .level = LogLevel::Info, .src = LogSource::Core}
    );
    spdlog::info(std::format("[CORE] -> {}", msg));
#endif
}
void KLog::Info(std::string msg) {
#ifdef KHAOS_DEBUG
    KLogger::AddMessageToBuffer(
        LogMessage{.msg = msg, .level = LogLevel::Info, .src = LogSource::Core}
    );
    spdlog::info(std::format("[CORE] -> {}", msg));
#endif
}

void KLog::Warn(const char* msg) {
#ifdef KHAOS_DEBUG
    KLogger::AddMessageToBuffer(
        LogMessage{.msg = msg, .level = LogLevel::Warning, .src = LogSource::Core}
    );
    spdlog::warn(std::format("[CORE] -> {}", msg));
#endif
}

void KLog::Warn(std::string msg) {
#ifdef KHAOS_DEBUG
    KLogger::AddMessageToBuffer(
        LogMessage{.msg = msg, .level = LogLevel::Warning, .src = LogSource::Core}
    );
    spdlog::warn(std::format("[CORE] -> {}", msg));
#endif
}

void KLog::Error(const char* msg) {
#ifdef KHAOS_DEBUG
    KLogger::AddMessageToBuffer(
        LogMessage{.msg = msg, .level = LogLevel::Error, .src = LogSource::Core}
    );
    spdlog::error(std::format("[CORE] -> {}", msg));
#endif
}
void KLog::Error(std::string msg) {
#ifdef KHAOS_DEBUG
    KLogger::AddMessageToBuffer(
        LogMessage{.msg = msg, .level = LogLevel::Error, .src = LogSource::Core}
    );
    spdlog::error(std::format("[CORE] -> {}", msg));
#endif
}
