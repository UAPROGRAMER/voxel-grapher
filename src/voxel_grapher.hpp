#pragma once

#include "graphics/camera.hpp"
#include "graphics/opengl.hpp"
#include "graphics/screen.hpp"
#include "graphics/shader.hpp"
#include "gui.hpp"
#include "variable.hpp"

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
  GUIparams guiParams;
  GUI gui;
  std::vector<Variable> variables;

  float lastScreenshotTime = 0;

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