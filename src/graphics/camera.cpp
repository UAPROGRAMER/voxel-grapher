#include "camera.hpp"

Camera::Camera(int width, int height)
    : pos(0, 0, 0),
      front(0, 0, -1),
      fov(45),
      lastX(width / 2.),
      lastY(height / 2.),
      yaw(-90),
      pitch(0),
      sensitivity(0.1),
      oldFocus(false) {}

void Camera::setFov(float newFov) { fov = newFov; }

float Camera::getFov() const { return fov; }

void Camera::setSensitivity(float newSensitivity) {
  sensitivity = newSensitivity;
}

float Camera::getSensitivity() const { return sensitivity; }

void Camera::moveForward(float length) { pos += length * front; }

void Camera::moveRight(float length) {
  pos += length * glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
}

void Camera::moveUp(float length) { pos += length * glm::vec3(0, 1, 0); }

glm::vec3 Camera::getPos() const { return pos; }

void Camera::mouse(double xpos, double ypos, bool focus) {
  if (oldFocus != focus) {
    lastX = xpos;
    lastY = ypos;

    oldFocus = focus;
  }

  if (focus == false) return;

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

float Camera::getYaw() const { return yaw; }

float Camera::getPitch() const { return pitch; }

glm::vec3 Camera::getFront() const { return front; }

glm::vec3 Camera::getRight() const {
  return glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
}

glm::vec3 Camera::getUp() const {
  return glm::normalize(
      glm::cross(glm::cross(front, glm::vec3(0, 1, 0)), front));
}