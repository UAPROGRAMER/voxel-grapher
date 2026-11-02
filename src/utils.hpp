#pragma once

#include <string>
#include <string_view>
#include <vector>

std::string readFileToString(std::string_view path);

void checkFolder(std::string_view path, bool strong);

void checkFile(std::string_view path);

void writeTextFile(std::string_view path, std::string_view str);

void createBmpImage(std::string_view path, uint32_t width, uint32_t height,
                    const std::vector<uint8_t>& data);

bool isValidId(std::string_view str);