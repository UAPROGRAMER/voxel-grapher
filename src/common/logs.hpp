#pragma once

#include <string_view>

enum class LogType {
    INFO,
    WARNING,
    ERROR,
};

void makeLog(LogType type, std::string_view msg);