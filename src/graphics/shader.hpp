#pragma once

#include "glm.hpp"
#include <string>

class Shader {
 private:
  unsigned int id;

 public:
  Shader(std::string vert, std::string frag);
  ~Shader() noexcept;

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  void use() const noexcept;

  int getUniformLocation(const std::string& name) const;

  void setUniformInt(const std::string& name, int v) const;

  void setUniformFloat(const std::string& name, float v) const;

  void setUniformVec3(const std::string& name, glm::vec3 v) const;

  void trySetUniformInt(const std::string& name, int v) const;

  void trySetUniformFloat(const std::string& name, float v) const;

  void trySetUniformVec3(const std::string& name, glm::vec3 v) const;
};