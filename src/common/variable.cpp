#include "variable.hpp"

#include <format>

Variable::Variable(std::string_view name) : name(name) {}

Variable::Variable(std::string&& name) : name(std::move(name)) {}

Variable::Variable(Variable&& other) noexcept
    : name(std::move(other.name)),
      min(other.min),
      max(other.max),
      step(other.step),
      value(other.value) {}

Variable& Variable::operator=(Variable&& other) noexcept {
  if (this != &other) {
    name = std::move(other.name);
    min = other.min;
    max = other.max;
    step = other.step;
    value = other.value;
  }

  return *this;
}

bool Variable::operator==(const Variable& other) const {
  return name == other.name;
}

std::string Variable::toString() const {
  return std::format("uniform float {};\n", name);
}