#include "camera.hpp"

Camera::Camera(int width, int height)
  : pos(0, 0, 0),
    front(0, 0, -1),
    fov(45),
    lastX(width / 2.),
    lastY(height / 2.),
    yaw(-90),
    pitch(0),
    speed(50),
    sensitivity(0.1) {}

void Camera::setSpeed(float newSpeed) {
  speed = newSpeed;
}

void Camera::moveForward(float delta, float dir) {
  pos += dir * speed * delta * front;
}

void Camera::moveSideways(float delta, float dir) {
  pos +=
    dir * speed * delta * glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
}

void Camera::moveUpways(float delta, float dir) {
  pos += dir * speed * delta * glm::vec3(0, 1, 0);
}

void Camera::mouse(double xpos, double ypos, bool focus) {
  static bool oldFocus = false;

  if (oldFocus != focus) {
    lastX = xpos;
    lastY = ypos;

    oldFocus = focus;
  }

  if (focus == false)
    return;

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch = std::max(std::min(pitch + yoffset, 89.9f), -89.9f);

  glm::vec3 direction;

  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  front = glm::normalize(direction);
}

void Camera::scroll(double xoffset, double yoffset) {
  fov = std::max(std::min(fov - (float)yoffset * 2, 120.f), 5.f);
}

void Camera::useUniforms(const Shader& shader) const {
  shader.setUniformVec3("cameraFront", front);
  shader.setUniformVec3(
    "cameraRight", glm::normalize(glm::cross(front, glm::vec3(0, 1, 0))));
  shader.setUniformVec3("cameraUp",
    glm::normalize(glm::cross(glm::cross(front, glm::vec3(0, 1, 0)), front)));
  shader.setUniformVec3("cameraPos", pos);
  shader.setUniformFloat("cameraFov", fov);
}

const glm::vec3& Camera::getPos() const {
  return pos;
}