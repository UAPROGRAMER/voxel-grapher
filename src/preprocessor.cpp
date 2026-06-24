#include "preprocessor.hpp"

PreprocArgs makePreprocArgs(const std::string& blockFunc,
  const std::string& displayFunc, const std::string& customVariables) {
  return {
    {'b', blockFunc      },
    {'d', displayFunc    },
    {'v', customVariables}
  };
}

std::string preprocess(const PreprocArgs& args, const std::string& str) {
  std::size_t size = 0;

  std::size_t i = 0;
  auto c = [&]() -> char { return i < str.size() ? str.at(i) : '\0'; };

  while (c() != '\0') {
    if (c() == '@') {
      i++;
      size += args.at(c()).size();
      i++;
    } else {
      size++;
      i++;
    }
  }

  std::string result;

  result.reserve(size);

  i = 0;

  while (c() != '\0') {
    if (c() == '@') {
      i++;
      result += args.at(c());
      i++;
    } else {
      result += c();
      i++;
    }
  }

  return result;
}