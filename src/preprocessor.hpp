#pragma once

#include <string>
#include <unordered_map>

using PreprocArgs = std::unordered_map<char, std::string>;

PreprocArgs makePreprocArgs(const std::string& blockFunc,
  const std::string& displayFunc, const std::string& customVariables);

std::string preprocess(const PreprocArgs& args, const std::string& str);