#pragma once

#include "opengl.hpp"
#include "shader.hpp"
#include "screen.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "variable.hpp"

class VoxelGrapher {
private:
  GLFWwindow* window;
  int width;
  int height;
  int renderWidth;
  int renderHeight;

  bool cameraMode;

  Shader renderShader;
  Shader displayShader;
  Screen screen;
  Camera camera;
  GUIparams guiParams;
  GUI gui;
  std::vector<Variable> variables;

  unsigned int renderTexture;
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