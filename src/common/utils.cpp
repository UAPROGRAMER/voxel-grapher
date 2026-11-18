#include "utils.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>
#include <string_view>

#include "logging.hpp"

std::string readFileToString(std::string_view path) {
  if (!std::filesystem::exists(path))
    makeLog(LogType::ERROR,
            std::format("[Utils]: Failed to read file: {}. File doesn't exist.",
                        path));

  if (!std::filesystem::is_regular_file(path))
    makeLog(LogType::ERROR,
            std::format("[Utils]: Failed to read file: {}. Not a file.", path));

  std::ifstream file((std::filesystem::path(path)));

  if (!file)
    makeLog(LogType::ERROR,
            std::format("[Utils]: Failed to open file: {}.", path));

  file.seekg(0, std::ios::end);

  size_t size = static_cast<size_t>(file.tellg());

  std::string text(size, '\0');

  file.seekg(0);

  file.read(text.data(), size);

  return text;
}

void checkFolder(std::string_view path, bool strong) {
  if (!std::filesystem::exists(path)) {
    if (strong)
      makeLog(LogType::ERROR,
              std::format("[Utils]: Expected folder: {}.", path));

    std::filesystem::create_directories(path);
  } else if (std::filesystem::is_regular_file(path))
    makeLog(LogType::ERROR,
            std::format("[Utils]: Expected folder: {}. Found file.", path));
}

void checkFile(std::string_view path) {
  if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
    makeLog(LogType::ERROR, std::format("[Utils]: Expected file {}.", path));
}

void writeTextFile(std::string_view path, std::string_view str) {
  std::ofstream file((std::filesystem::path(path)));

  if (!file)
    makeLog(LogType::ERROR,
            std::format("[Utils]: Failed to write txt file: {}.", path));

  file << str;

  file.flush();
}

bool isValidId(std::string_view str) {
  if (str.empty()) return false;

  if (!isalpha(str.at(0))) return false;

  for (char c : str)
    if (!isalnum(c) && c != '_') return false;

  return true;
}