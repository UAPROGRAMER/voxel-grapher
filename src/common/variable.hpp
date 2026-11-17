#pragma once

#include <string>
#include <string_view>

struct Variable {
  std::string name;
  float min = 0;
  float max = 1;
  float step = 0.01;
  float value = 0;

  Variable(std::string_view name);

  Variable(std::string&& name);

  Variable(const Variable&) = delete;
  Variable& operator=(const Variable&) = delete;

  Variable(Variable&& other) noexcept;
  Variable& operator=(Variable&& other) noexcept;

  std::string toString() const;
};