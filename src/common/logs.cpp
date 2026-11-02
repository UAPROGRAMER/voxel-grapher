#include "logs.hpp"
#include <exception>
#include <format>
#include <fstream>

void makeLog(LogType type, std::string_view msg) {
    static std::ofstream logFile(std::format("./data/logs/{}.log", std::time(nullptr)));

    switch (type) {
        case LogType::INFO:
            logFile << "[INFO]: ";
            break;
        case LogType::WARNING:
            logFile << "[WARNING]: ";
            break;
        case LogType::ERROR:
            logFile << "[ERROR]: ";
            break;
    }

    logFile << msg << '\n';

    logFile.flush();

    if (type == LogType::ERROR) throw std::exception();
}