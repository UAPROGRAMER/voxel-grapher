#pragma once

#include <string>
#include <filesystem>

std::string readFileToString(const std::filesystem::path& path);

void writeLogFile(const std::string& str) noexcept;

void checkFolder(const std::filesystem::path& path, bool strong);

void checkFile(const std::filesystem::path& path);