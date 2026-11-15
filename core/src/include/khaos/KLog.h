#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace KhaosCore {
    constexpr unsigned int MAX_LOG_MESSAGE_COUNT = 500;
    enum class LogLevel : std::uint8_t { Info, Warning, Error };

    enum class LogSource : std::uint8_t { Core, Editor, Runtime };

    struct LogMessage {
        std::string msg;
        LogLevel level;
        LogSource src;
    };

    class KLogger {
      private:
        std::vector<LogMessage> mBuffer;

      public:
        static KLogger& instance() {
            static KLogger sInstance;
            return sInstance;
        };
        static void AddMessageToBuffer(LogMessage message);
        static const std::vector<LogMessage>& GetBuffer();
    };
}; // namespace KhaosCore
