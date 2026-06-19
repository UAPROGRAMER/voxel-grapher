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