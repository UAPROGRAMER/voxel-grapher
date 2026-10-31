#pragma once

#include "glm.hpp"

class Camera {
 private:
  glm::vec3 pos;
  glm::vec3 front;
  float fov;

  float lastX;
  float lastY;

  float yaw;
  float pitch;

  float sensitivity;

  bool oldFocus;

 public:
  Camera(int width, int height);

  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  void setFov(float fov);

  float getFov() const;

  void setSensitivity(float sensitivity);

  float getSensitivity() const;

  void moveForward(float length);

  void moveRight(float length);

  void moveUp(float length);

  glm::vec3 getPos() const;

  void mouse(double xpos, double ypos, bool focus);

  float getYaw() const;

  float getPitch() const;

  glm::vec3 getFront() const;

  glm::vec3 getRight() const;

  glm::vec3 getUp() const;
};