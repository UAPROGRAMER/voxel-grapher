#include "variable.hpp"

Variable::Variable(const std::string& name) : name(name) {}

std::string Variable::toString() const {
  return "uniform float " + name + ";\n";
}