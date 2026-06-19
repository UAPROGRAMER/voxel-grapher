#include "voxel_grapher.hpp"
#include "imgui.hpp"
#include "error.hpp"
#include "utils.hpp"

PreprocArgs makePreprocArgs(
  const std::string& blockFunc, const std::string& displayFunc) {
  return {
    {'b', blockFunc  },
    {'d', displayFunc}
  };
}

VoxelGrapher::VoxelGrapher(GLFWwindow* window, int width, int height)
  : window(window),
    width(width),
    cameraMode(false),
    height(height),
    renderShader("./data/shaders/render.vert", "./data/shaders/render.frag",
      makePreprocArgs(defaultBlockFunc, defaultDisplayFunc)),
    displayShader("./data/shaders/display.vert", "./data/shaders/display.frag"),
    screen(renderTexture, width, height),
    camera(width, height),
    guiParams(defaultBlockFunc, defaultDisplayFunc),
    gui(guiParams) {}

void VoxelGrapher::run() {
  float lastTime;
  float time = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    lastTime = time;
    time = glfwGetTime();
    float delta = time - lastTime;

    process(time, delta);

    draw(time, delta);

    glfwSwapBuffers(window);
  }
}

void VoxelGrapher::process(float time, float delta) {
  if (!ImGui::GetIO().WantCaptureKeyboard) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      if (!cameraMode) {
        cameraMode = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
    } else {
      if (cameraMode) {
        cameraMode = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    }

    if (cameraMode) {
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(delta, 1);

      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveForward(delta, -1);

      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveSideways(delta, 1);

      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveSideways(delta, -1);

      if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.moveUpways(delta, 1);

      if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.moveUpways(delta, -1);

      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.setSpeed(250);
      else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.setSpeed(10);
      else camera.setSpeed(50);
    }
  }

  gui.process(time, delta, cameraMode);

  if (guiParams.resetFuncs) {
    guiParams.blockFunc = defaultBlockFunc;
    guiParams.displayFunc = defaultDisplayFunc;

    gui.resetFuncs();

    guiParams.resetFuncs = false;
  }

  if (guiParams.compileFuncs) {
    try {
      Shader newShader("./data/shaders/render.vert",
        "./data/shaders/render.frag",
        makePreprocArgs(guiParams.blockFunc, guiParams.displayFunc));

      renderShader = std::move(newShader);

      guiParams.isFuncError = false;
    } catch (const VoxelGrapherError& e) {
      guiParams.isFuncError = true;

      guiParams.funcError = "Error: " + e.msg;
    }

    guiParams.compileFuncs = false;
  }

  if (guiParams.isRenderScaleChanged) {
    glBindTexture(GL_TEXTURE_2D, renderTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      (int)(width * guiParams.renderScale),
      (int)(height * guiParams.renderScale), 0, GL_RGBA, GL_UNSIGNED_BYTE,
      NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    guiParams.isRenderScaleChanged = false;
  }

  if (guiParams.interpolationChanged) {
    glBindTexture(GL_TEXTURE_2D, renderTexture);

    if (guiParams.interpolateRender) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    guiParams.interpolationChanged = false;
  }

  if (guiParams.isLoad) {
    guiParams.isLoad = false;

    guiParams.blockFunc =
      readFileToString("./data/saves/" + guiParams.savefile + "/block.txt");
    guiParams.displayFunc =
      readFileToString("./data/saves/" + guiParams.savefile + "/display.txt");

    gui.resetFuncs();
  }

  if (guiParams.isSave) {
    guiParams.isSave = false;

    std::filesystem::create_directory("./data/saves/" + guiParams.savefile);

    writeTextFile(
      "./data/saves/" + guiParams.savefile + "/block.txt", guiParams.blockFunc);
    writeTextFile("./data/saves/" + guiParams.savefile + "/display.txt",
      guiParams.displayFunc);
  }

  if (guiParams.isDelete) {
    guiParams.isDelete = false;

    std::filesystem::remove_all("./data/saves/" + guiParams.savefile);
  }
}

void VoxelGrapher::draw(float time, float delta) const {
  glViewport(0, 0, width, height);

  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  renderShader.use();

  renderShader.setUniformInt("width", width);
  renderShader.setUniformInt("height", height);
  camera.useUniforms(renderShader);
  renderShader.setUniformFloat("maxRayLength", guiParams.maxRayLength);

  glViewport(0, 0, (int)(width * guiParams.renderScale),
    (int)(height * guiParams.renderScale));

  screen.render();

  glViewport(0, 0, width, height);

  displayShader.use();

  glBindTexture(GL_TEXTURE_2D, renderTexture);

  screen.draw();

  glBindTexture(GL_TEXTURE_2D, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  gui.draw(time, delta);
}

void VoxelGrapher::windowResize(int _width, int _height) {
  width = _width;
  height = _height;

  glViewport(0, 0, width, height);

  glBindTexture(GL_TEXTURE_2D, renderTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)(width * guiParams.renderScale),
    (int)(height * guiParams.renderScale), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void VoxelGrapher::mouse(double xpos, double ypos) {
  if (ImGui::GetIO().WantCaptureMouse) return;

  camera.mouse(
    xpos, ypos, glfwGetWindowAttrib(window, GLFW_FOCUSED) && cameraMode);
}

void VoxelGrapher::scroll(double xoffset, double yoffset) {
  if (cameraMode) camera.scroll(xoffset, yoffset);
}