#include "shader.hpp"
#include "opengl.hpp"
#include "utils.hpp"
#include "error.hpp"

Shader::Shader(const std::string& vertPath, const std::string& fragPath,
  const PreprocArgs& args) {
  std::string vert = readFileToString(vertPath);
  std::string frag = readFileToString(fragPath);

  if (!args.empty()) frag = preprocess(args, frag);

  int success;
  char infoLog[512];

  // Vertex

  const char* vertSource = vert.c_str();

  unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertShader, 1, &vertSource, nullptr);
  glCompileShader(vertShader);

  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
    throw VoxelGrapherError(
      "Failed to compile vertex shader: " + std::string(infoLog));
  }

  // Fragment

  const char* fragSource = frag.c_str();

  unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragShader, 1, &fragSource, nullptr);
  glCompileShader(fragShader);

  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
    throw VoxelGrapherError(
      "Failed to compile fragment shader: " + std::string(infoLog));
  }

  // Program

  unsigned int program = glCreateProgram();

  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);

  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    throw VoxelGrapherError(
      "Failed to link shader program: " + std::string(infoLog));
  }

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  id = program;
}

Shader::~Shader() noexcept {
  if (id != 0) glDeleteProgram(id);
}

Shader::Shader(Shader&& other) noexcept : id(other.id) { other.id = 0; }

Shader& Shader::operator=(Shader&& other) noexcept {
  if (this != &other) {
    id = other.id;
    other.id = 0;
  }

  return *this;
}

void Shader::use() const noexcept { glUseProgram(id); }

int Shader::getUniformLocation(const std::string& name) const {
  use();

  int location = glGetUniformLocation(id, name.c_str());

  if (location == -1)
    throw VoxelGrapherError("Failed to find uniform \"" + name + "\".");

  return location;
}

void Shader::setUniformInt(const std::string& name, int v) const {
  use();

  glUniform1i(getUniformLocation(name), v);
}

void Shader::setUniformFloat(const std::string& name, float v) const {
  use();

  glUniform1f(getUniformLocation(name), v);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& v) const {
  use();

  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(v));
}

void Shader::trySetUniformInt(const std::string& name, int v) const {
  use();

  int location = glGetUniformLocation(id, name.c_str());

  if (location == -1) return;

  glUniform1i(location, v);
}

void Shader::trySetUniformFloat(const std::string& name, float v) const {
  use();

  int location = glGetUniformLocation(id, name.c_str());

  if (location == -1) return;

  glUniform1f(location, v);
}

void Shader::trySetUniformVec3(const std::string& name, const glm::vec3& v) const {
  use();

  int location = glGetUniformLocation(id, name.c_str());

  if (location == -1) return;

  glUniform3fv(location, 1, glm::value_ptr(v));
}