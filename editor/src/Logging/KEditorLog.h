#pragma once

#include <string>
namespace KLog {
    void Info(const char* msg);
    void Info(std::string msg);
    void Warn(const char* msg);
    void Warn(std::string msg);
    void Error(const char* msg);
    void Error(std::string msg);

} // namespace KLog
