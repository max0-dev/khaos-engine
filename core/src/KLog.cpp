#include "khaos/KLog.h"
#include <fmt/base.h>
#include <vector>

using namespace KhaosCore;

void KLogger::AddMessageToBuffer(LogMessage message) {
    if (instance().mBuffer.capacity() >= MAX_LOG_MESSAGE_COUNT) {
        instance().mBuffer.clear();
    }
    instance().mBuffer.emplace_back(message);
}

const std::vector<LogMessage>& KLogger::GetBuffer() { return instance().mBuffer; }
