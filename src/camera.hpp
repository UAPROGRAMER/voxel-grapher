#pragma once

#include "math.hpp"
#include "shader.hpp"

class Camera {
private:
  glm::vec3 pos;
  glm::vec3 front;
  float fov;

  float lastX;
  float lastY;

  float yaw;
  float pitch;

  float speed;
  float sensitivity;

public:
  Camera(int width, int height);

  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  void setSpeed(float speed);

  void moveForward(float delta, float dir);

  void moveSideways(float delta, float dir);

  void moveUpways(float delta, float dir);

  void mouse(double xpos, double ypos, bool focus);

  void scroll(double xoffset, double yoffset);

  void useUniforms(const Shader& shader) const;
};