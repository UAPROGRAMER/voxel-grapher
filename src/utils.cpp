#include "utils.hpp"
#include "error.hpp"

#include <fstream>

std::string readFileToString(const std::filesystem::path& path) {
  if (!std::filesystem::exists(path))
    throw VoxelGrapherError(
      "Failed to read a file \"" + path.string() + "\" to string. No file.");

  if (!std::filesystem::is_regular_file(path))
    throw VoxelGrapherError("Failed to read a file \"" + path.string() +
      "\" to string. Not a regular file.");

  std::ifstream file(path);

  if (!file)
    throw VoxelGrapherError("Failed to read a file \"" + path.string() +
      "\" to string. Failed to open a file.");

  file.seekg(0, std::ios::end);

  size_t size = static_cast<size_t>(file.tellg());

  std::string text(size, '\0');

  file.seekg(0);

  file.read(text.data(), size);

  return text;
}

void writeLogFile(const std::string& str) noexcept {
  try {
    std::ofstream file(
      "./data/logs/log-" + std::to_string(std::time(nullptr)) + ".log");

    file << str;

    file.flush();
  } catch (...) {}
}

void checkFolder(const std::filesystem::path& path, bool strong) {
  if (!std::filesystem::exists(path)) {
    if (strong)
      throw VoxelGrapherError("\"" + path.string() + "\" folder is required.");

    std::filesystem::create_directories(path);
  } else if (std::filesystem::is_regular_file(path))
    throw VoxelGrapherError("\"" + path.string() + "\" must be a folder.");
}

void checkFile(const std::filesystem::path& path) {
  if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
    throw VoxelGrapherError("\"" + path.string() + "\" is required.");
}

void writeTextFile(const std::filesystem::path& path, const std::string& str) {
  std::ofstream file(path);

  if (!file)
    throw VoxelGrapherError("Failed to create a file \"" + path.string() +
      "\". Failed to open a file.");

  file << str;
}

void createBmpImage(const std::filesystem::path& path, uint32_t width,
  uint32_t height, const std::vector<uint8_t>& data) {
  if (((uint64_t)(width) * (uint64_t)(height) * 3 + 54) !=
    (uint32_t)((uint64_t)(width) * (uint64_t)(height) * 3 + 54))
    throw VoxelGrapherError("Failed to create .bmp image \"" + path.string() +
      "\". Size is too big.");

  std::ofstream file(path, std::ios::binary);

  if (!file)
    throw VoxelGrapherError("Failed to create .bmp image \"" + path.string() +
      "\". Failed to open a file.");

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

bool isValidId(const std::string& str) {
  if (str.empty()) return false;

  if (!isalpha(str.at(0))) return false;

  for (char c : str)
    if (!isalnum(c) && c != '_') return false;

  return true;
}