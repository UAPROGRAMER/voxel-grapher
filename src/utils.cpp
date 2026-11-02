#include "utils.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>
#include <string_view>

#include "common/logs.hpp"

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

void createBmpImage(std::string_view path, uint32_t width, uint32_t height,
                    const std::vector<uint8_t>& data) {
  if (((uint64_t)(width) * (uint64_t)(height) * 3 + 54) !=
      (uint32_t)((uint64_t)(width) * (uint64_t)(height) * 3 + 54))
    makeLog(LogType::ERROR,
            std::format(
                "[Utils]: Failed to create bmp image: {}. Image is too big.",
                path));

  std::ofstream file((std::filesystem::path(path)), std::ios::binary);

  if (!file)
    makeLog(LogType::ERROR,
            std::format(
                "[Utils]: Failed to create bmp image: {}. Failed to open file.",
                path));

  auto writeByte = [&](uint8_t data) -> void {
    file.write(reinterpret_cast<const char*>(&data), sizeof(data));
  };

  auto write2Bytes = [&](uint16_t data) -> void {
    writeByte(data);
    writeByte(data >> 8);
  };

  auto write4Bytes = [&](uint32_t data) -> void {
    writeByte(data);
    writeByte(data >> 8);
    writeByte(data >> 16);
    writeByte(data >> 24);
  };

  // Header (54 bytes)
  const uint32_t padding = (4 - (width * 3) % 4) % 4;
  const uint32_t dataSize = (width * 3) * height;

  writeByte('B');
  writeByte('M');

  write4Bytes(dataSize + 54);

  write2Bytes(0);
  write2Bytes(0);

  write4Bytes(0x36);
  write4Bytes(0x28);

  write4Bytes(width);
  write4Bytes(height);

  write2Bytes(0x01);
  write2Bytes(0x18);

  write4Bytes(0);

  write4Bytes(dataSize);

  write4Bytes(0x0EC4);
  write4Bytes(0x0EC4);

  write4Bytes(0);
  write4Bytes(0);

  // Data
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width * 3; x++)
      writeByte(data.at(y * width * 3 + x));

    for (size_t i = 0; i < padding; i++) writeByte(0);
  }
}

bool isValidId(std::string_view str) {
  if (str.empty()) return false;

  if (!isalpha(str.at(0))) return false;

  for (char c : str)
    if (!isalnum(c) && c != '_') return false;

  return true;
}