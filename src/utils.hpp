#pragma once

#include <string>
#include <filesystem>
#include <cinttypes>
#include <vector>

std::string readFileToString(const std::filesystem::path& path);

void writeLogFile(const std::string& str) noexcept;

void checkFolder(const std::filesystem::path& path, bool strong);

void checkFile(const std::filesystem::path& path);

void writeTextFile(const std::filesystem::path& path, const std::string& str);

void createBmpImage(const std::filesystem::path& path,
  uint32_t width, uint32_t height, const std::vector<uint8_t>& data);