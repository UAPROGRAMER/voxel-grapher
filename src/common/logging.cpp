#include "logging.hpp"

#include <exception>
#include <format>
#include <fstream>
#include <sstream>

void makeLog(LogType type, std::string_view msg) {
  static std::stringstream logStream;

  switch (type) {
    case LogType::INFO:
      logStream << "[INFO]: ";
      break;
    case LogType::WARNING:
      logStream << "[WARNING]: ";
      break;
    case LogType::ERROR:
      logStream << "[ERROR]: ";
      break;
  }

  logStream << msg << '\n';

  if (type == LogType::ERROR) {
    static std::ofstream logFile(
        std::format("./data/logs/{}.log", std::time(nullptr)));

    logFile << logStream.rdbuf();

    throw std::exception();
  }
}