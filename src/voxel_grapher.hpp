#pragma once

#include <string>

#include "common/theme.hpp"
#include "common/variable.hpp"
#include "graphics/camera.hpp"
#include "graphics/opengl.hpp"
#include "graphics/screen.hpp"
#include "graphics/shader.hpp"
#include "gui/gui.hpp"

class VoxelGrapher {
 private:
  GLFWwindow* window;
  int windowed_width;
  int windowed_height;
  int width;
  int height;
  int posX;
  int posY;
  int renderWidth;
  int renderHeight;
  bool fullscreen;
  float lastFullscreenTime = 0;

  bool cameraMode;
  float cameraSpeed;

  Shader renderShader;
  Shader displayShader;
  Screen screen;
  Camera camera;
  GUI gui;
  std::vector<Variable> variables;

  float lastScreenshotTime = 0;
  bool screenshot = false;
  float maxRayLength = 40;
  float renderScale = 1;
  bool interpolation = false;

  bool lastError = false;
  std::string lastErrorText = "";

  Theme theme;

  void process(float time, float delta);

  void draw(float time, float delta) const;

 public:
  VoxelGrapher(GLFWwindow* window, int width, int height);

  VoxelGrapher(const VoxelGrapher&) = delete;
  VoxelGrapher& operator=(const VoxelGrapher&) = delete;

  void run();

  void windowResize(int width, int height);

  void mouse(double xpos, double ypos);

  void scroll(double xoffset, double yoffset);
};