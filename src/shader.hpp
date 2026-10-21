#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "preprocessor.hpp"

const std::string defaultBlockFunc = "return 0;";

const std::string defaultDisplayFunc =  //
    "switch (lastCoord) {\n"
    "  case 0:\n"
    "    return vec4(0, 0, 0, 1);\n"
    "  case 1:\n"
    "    return vec4(0.9, 0.2, 0.2,\n"
    "      min(1, (maxRayLength - dist) * 5));\n"
    "  case 2:\n"
    "    return vec4(0.2, 0.9, 0.2,\n"
    "      min(1, (maxRayLength - dist) * 5));\n"
    "  case 3:\n"
    "    return vec4(0.2, 0.2, 0.9,\n"
    "      min(1, (maxRayLength - dist) * 5));\n"
    "}";

class Shader {
 private:
  unsigned int id;

 public:
  Shader(const std::string& vertPath, const std::string& fragPath,
         const PreprocArgs& args = {});
  ~Shader() noexcept;

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  void use() const noexcept;

  int getUniformLocation(const std::string& name) const;

  void setUniformInt(const std::string& name, int v) const;

  void setUniformFloat(const std::string& name, float v) const;

  void setUniformVec3(const std::string& name, const glm::vec3& v) const;

  void trySetUniformInt(const std::string& name, int v) const;

  void trySetUniformFloat(const std::string& name, float v) const;

  void trySetUniformVec3(const std::string& name, const glm::vec3& v) const;
};