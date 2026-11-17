#include "voxel_grapher.hpp"

#include <mini/ini.h>

#include <exception>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <tuple>

#include "common/logs.hpp"
#include "common/preprocessor.hpp"
#include "common/theme.hpp"
#include "common/utils.hpp"
#include "graphics/imgui.hpp"
#include "graphics/opengl.hpp"
#include "gui.hpp"

constexpr std::size_t MAX_VARIABLES = 256;

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

VoxelGrapher::VoxelGrapher(GLFWwindow* window, int width, int height)
    : window(window),
      windowed_width(0),
      windowed_height(0),
      width(width),
      height(height),
      posX(0),
      posY(0),
      renderWidth(width),
      renderHeight(height),
      fullscreen(false),
      cameraMode(false),
      cameraSpeed(50),
      renderShader(
          readFileToString("./data/shaders/render.vert"),
          preprocess(makePreprocArgs(defaultBlockFunc, defaultDisplayFunc, ""),
                     readFileToString("./data/shaders/render.frag"))),
      displayShader(readFileToString("./data/shaders/display.vert"),
                    readFileToString("./data/shaders/display.frag")),
      screen(width, height, width, height),
      camera(width, height),
      gui(defaultBlockFunc, defaultDisplayFunc),
      variables() {
  glfwGetWindowSize(window, &width, &height);
  glfwGetWindowPos(window, &posX, &posY);

  if (std::filesystem::exists("./data/theme-light"))
    theme = Theme::LIGHT;
  else if (std::filesystem::exists("./data/theme-dark"))
    theme = Theme::DARK;
  else if (std::filesystem::exists("./data/theme-classic"))
    theme = Theme::CLASSIC;
}

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
  GUIBusIn busIn(variables);

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

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) screenshot = true;

    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS &&
        lastFullscreenTime + 0.5 <= time) {
      if (fullscreen) {
        glfwSetWindowMonitor(window, nullptr, posX, posY, windowed_width,
                             windowed_height, GLFW_DONT_CARE);

        windowResize(height, width);

        fullscreen = false;
      } else {
        glfwGetWindowSize(window, &width, &height);
        glfwGetWindowPos(window, &posX, &posY);

        windowed_width = width;
        windowed_height = height;

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width,
                             mode->height, mode->refreshRate);

        windowResize(mode->width, mode->height);

        fullscreen = true;
      }

      lastFullscreenTime = time;
    }

    if (cameraMode) {
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(delta * cameraSpeed);

      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveForward(delta * -cameraSpeed);

      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(delta * cameraSpeed);

      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRight(delta * -cameraSpeed);

      if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.moveUp(delta * cameraSpeed);

      if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.moveUp(delta * -cameraSpeed);

      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraSpeed = 250;
      else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 10;
      else
        cameraSpeed = 50;
    }

    if (!cameraMode) {
      if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) busIn.openHelp = true;

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) busIn.openInfo = true;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
          busIn.openParams = true;

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
          busIn.openFunctions = true;

        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
          busIn.openVariables = true;

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) busIn.openLoad = true;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) busIn.openSave = true;

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
          std::filesystem::remove("./data/theme-light");
          std::filesystem::remove("./data/theme-dark");

          std::ignore = std::ofstream("./data/theme-classic");

          glfwSetWindowShouldClose(window, true);

          theme = Theme::CLASSIC;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
          std::filesystem::remove("./data/theme-classic");
          std::filesystem::remove("./data/theme-dark");

          std::ignore = std::ofstream("./data/theme-light");

          glfwSetWindowShouldClose(window, true);

          theme = Theme::LIGHT;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
          std::filesystem::remove("./data/theme-classic");
          std::filesystem::remove("./data/theme-light");

          std::ignore = std::ofstream("./data/theme-dark");

          glfwSetWindowShouldClose(window, true);

          theme = Theme::DARK;
        }
      }
    }
  }

  busIn.cameraMode = cameraMode;

  busIn.cameraPos = camera.getPos();
  busIn.yaw = camera.getYaw();
  busIn.pitch = camera.getPitch();
  busIn.fov = camera.getFov();
  busIn.sensitivity = camera.getSensitivity();

  busIn.time = time;

  busIn.maxRayLength = maxRayLength;
  busIn.renderScale = renderScale;
  busIn.interpolation = interpolation;

  busIn.theme = theme;

  if (lastError) {
    busIn.error = true;
    busIn.errorText = lastErrorText;
  }

  const GUIBusOut busOut = gui.process(busIn);

  maxRayLength = busOut.maxRayLength;
  renderScale = busOut.renderScale;
  interpolation = busOut.interpolation;

  if (busOut.takeScreenshot) screenshot = true;

  // Chenking data GUI sent
  if (screenshot && lastScreenshotTime + 2 <= time) {
    lastScreenshotTime = time;

    createBmpImage(std::format("./data/screenshots/{}.bmp",
                               std::to_string(std::time(nullptr))),
                   renderWidth, renderHeight, screen.getPixels());
  }

  if (busOut.resetProgram) gui.resetFuncs(defaultBlockFunc, defaultDisplayFunc);

  if (busOut.compileProgram) {
    try {
      std::string customVariables;
      for (const auto& variable : variables)
        customVariables += variable.toString();

      Shader newShader(
          readFileToString("./data/shaders/render.vert"),
          preprocess(makePreprocArgs(busOut.blockFunc, busOut.displayFunc,
                                     customVariables),
                     readFileToString("./data/shaders/render.frag")));

      renderShader = std::move(newShader);

      lastError = false;
    } catch (const std::exception& e) {
      lastError = true;
      lastErrorText = std::format("Error: {}", e.what());
    }
  }

  if (busOut.renderScaleChanged) {
    renderWidth = (int)(width * busOut.renderScale);
    renderHeight = (int)(height * busOut.renderScale);

    screen.changeSize(renderWidth, renderHeight, width, height);
  }

  if (busOut.interpolationChanged) {
    if (busOut.interpolation)
      screen.setFilterLinear();
    else
      screen.setFilterNearest();
  }

  if (busOut.loadSavefile) {
    variables.clear();

    std::string blockFunc = readFileToString(
        std::format("./data/saves/{}/block.txt", busOut.savefile));
    std::string displayFunc = readFileToString(
        std::format("./data/saves/{}/display.txt", busOut.savefile));

    mINI::INIFile varsFile(
        std::format("./data/saves/{}/vars.ini", busOut.savefile));

    mINI::INIStructure ini;

    varsFile.read(ini);

    for (const auto& variable : ini) {
      if (variable.first.empty()) continue;

      variables.push_back(Variable(variable.first));
      variables.at(variables.size() - 1).value =
          std::stof(variable.second.get("value"));
      variables.at(variables.size() - 1).step =
          std::stof(variable.second.get("step"));
      variables.at(variables.size() - 1).min =
          std::stof(variable.second.get("min"));
      variables.at(variables.size() - 1).max =
          std::stof(variable.second.get("max"));
    }

    gui.resetFuncs(blockFunc, displayFunc);

    makeLog(LogType::INFO,
            std::format("Loaded savefile \"{}\".", busOut.savefile));
  }

  if (busOut.saveSavefile) {
    std::filesystem::create_directory(
        std::format("./data/saves/{}", busOut.savefile));

    writeTextFile(std::format("./data/saves/{}/block.txt", busOut.savefile),
                  busOut.blockFunc);
    writeTextFile(std::format("./data/saves/{}/display.txt", busOut.savefile),
                  busOut.displayFunc);

    mINI::INIFile varsFile(
        std::format("./data/saves/{}/vars.ini", busOut.savefile));

    mINI::INIStructure ini;

    for (const auto& variable : variables) {
      ini[variable.name]["value"] = std::to_string(variable.value);
      ini[variable.name]["step"] = std::to_string(variable.step);
      ini[variable.name]["min"] = std::to_string(variable.min);
      ini[variable.name]["max"] = std::to_string(variable.max);
    }

    varsFile.generate(ini);

    makeLog(LogType::INFO, std::format("Saved file \"{}\".", busOut.savefile));
  }

  if (busOut.deleteSavefile) {
    std::filesystem::remove_all(
        std::format("./data/saves/{}", busOut.savefile));
    makeLog(LogType::INFO,
            std::format("Deleted file \"{}\".", busOut.savefile));
  }

  if (busOut.cameraChanged) {
    camera.setFov(busOut.fov);
    camera.setSensitivity(busOut.sensitivity);
  }

  if (busOut.variableValueChanged)
    variables.at(busOut.variableIndex).value = busOut.variableValue;

  if (busOut.variableChanged) {
    variables.at(busOut.variableIndex).value =
        std::max(busOut.variableMin, variables.at(busOut.variableIndex).value);
    variables.at(busOut.variableIndex).step = busOut.variableStep;
    variables.at(busOut.variableIndex).min = busOut.variableMin;
    variables.at(busOut.variableIndex).max = busOut.variableMax;
  }

  if (busOut.variableAdded && variables.size() < MAX_VARIABLES) {
    variables.emplace_back(busOut.variableName);
    variables.at(variables.size() - 1).step = busOut.variableStep;
    variables.at(variables.size() - 1).min = busOut.variableMin;
    variables.at(variables.size() - 1).max = busOut.variableMax;
    variables.at(variables.size() - 1).value = busOut.variableMin;
  }

  if (busOut.variableDeleted)
    variables.erase(variables.begin() + busOut.variableIndex);

  if (busOut.themeChanged) {
    if (busOut.theme == Theme::CLASSIC) {
      std::filesystem::remove("./data/theme-light");
      std::filesystem::remove("./data/theme-dark");

      std::ignore = std::ofstream("./data/theme-classic");

      glfwSetWindowShouldClose(window, true);

      theme = Theme::CLASSIC;
    } else if (busOut.theme == Theme::LIGHT) {
      std::filesystem::remove("./data/theme-classic");
      std::filesystem::remove("./data/theme-dark");

      std::ignore = std::ofstream("./data/theme-light");

      glfwSetWindowShouldClose(window, true);

      theme = Theme::LIGHT;
    } else if (busOut.theme == Theme::DARK) {
      std::filesystem::remove("./data/theme-classic");
      std::filesystem::remove("./data/theme-light");

      std::ignore = std::ofstream("./data/theme-dark");

      glfwSetWindowShouldClose(window, true);

      theme = Theme::DARK;
    } else
      makeLog(LogType::ERROR, "Failed to find theme.");
  }
}

void VoxelGrapher::draw(float time, float delta) const {
  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  renderShader.use();

  renderShader.trySetUniformInt("width", width);
  renderShader.trySetUniformInt("height", height);

  renderShader.trySetUniformVec3("cameraFront", camera.getFront());
  renderShader.trySetUniformVec3("cameraRight", camera.getRight());
  renderShader.trySetUniformVec3("cameraUp", camera.getUp());
  renderShader.trySetUniformVec3("cameraPos", camera.getPos());
  renderShader.trySetUniformFloat("cameraFov", camera.getFov());

  renderShader.trySetUniformFloat("maxRayLength", maxRayLength);
  renderShader.trySetUniformFloat("t", time);

  for (const auto& variable : variables)
    renderShader.trySetUniformFloat(variable.name, variable.value);

  screen.render();

  displayShader.use();

  screen.draw();

  glUseProgram(0);

  gui.draw();
}

void VoxelGrapher::windowResize(int _width, int _height) {
  width = _width;
  height = _height;
  renderWidth = (int)(width * renderScale);
  renderHeight = (int)(height * renderScale);

  glViewport(0, 0, width, height);

  screen.changeSize(renderWidth, renderHeight, width, height);
}

void VoxelGrapher::mouse(double xpos, double ypos) {
  if (ImGui::GetIO().WantCaptureMouse) return;

  camera.mouse(xpos, ypos,
               glfwGetWindowAttrib(window, GLFW_FOCUSED) && cameraMode);
}

void VoxelGrapher::scroll(double xoffset, double yoffset) {
  if (cameraMode)
    camera.setFov(
        std::max(std::min(camera.getFov() - (float)yoffset * 2, 120.f), 5.f));
}