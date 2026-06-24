#pragma once

#include <string>

struct Variable {
  std::string name;
  float min = 0;
  float max = 1;
  float step = 0.01;
  float value = 0;

  Variable(const std::string& name);

  std::string toString() const;
};