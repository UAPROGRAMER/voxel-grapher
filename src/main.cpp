#include "opengl.hpp"
#include "voxel_grapher.hpp"
#include "utils.hpp"
#include "error.hpp"
#include <stb/stb_image.h>
#include "imgui.hpp"

#include <optional>

constexpr int defaultWidth = 800;
constexpr int defaultHeight = 800;

std::optional<VoxelGrapher> voxelGrapher;

void framebufferSizeCallback(GLFWwindow* window, int newWidth, int newHeight) {
  if (voxelGrapher.has_value())
    voxelGrapher.value().windowResize(newWidth, newHeight);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  if (voxelGrapher.has_value()) voxelGrapher.value().mouse(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  if (voxelGrapher.has_value()) voxelGrapher.value().scroll(xoffset, yoffset);
}

int main() {
  try {
    // Check folders and files

    checkFolder("./data", true);
    checkFolder("./data/shaders", true);
    checkFolder("./data/icons", true);

    checkFolder("./data/logs", false);
    checkFolder("./data/saves", false);
    checkFolder("./data/screenshots", false);

    checkFile("./data/shaders/render.vert");
    checkFile("./data/shaders/render.frag");
    checkFile("./data/shaders/display.vert");
    checkFile("./data/shaders/display.frag");

    checkFile("./data/icons/icon512.png");
    checkFile("./data/icons/icon256.png");
    checkFile("./data/icons/icon128.png");
    checkFile("./data/icons/icon.ico");

    // Glfw init

    glfwInit();

    // Glfw setup

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    glfwWindowHintString(
      GLFW_WAYLAND_APP_ID, "cc.stanislavyatskiv.voxel-grapher");

// Some apple magic (as always)
#if defined(__APPLE__) && defined(__MACH__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /*
    #if defined(__linux__)
        glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    #endif
    */

    // Creating window

    GLFWwindow* window = glfwCreateWindow(
      defaultWidth, defaultHeight, "Voxel Grapher", nullptr, nullptr);

    if (!window) return 1;

    glfwMakeContextCurrent(window);

    // GLFW Settings
    glfwSwapInterval(1);

    // Callbacks

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Window icon

    GLFWimage windowIcon[3];
    int nrChannels;

    windowIcon[0].pixels = stbi_load("./data/icons/icon512.png",
      &(windowIcon[0].width), &(windowIcon[0].height), &nrChannels, 0);

    windowIcon[1].pixels = stbi_load("./data/icons/icon256.png",
      &(windowIcon[1].width), &(windowIcon[1].height), &nrChannels, 0);

    windowIcon[2].pixels = stbi_load("./data/icons/icon128.png",
      &(windowIcon[2].width), &(windowIcon[2].height), &nrChannels, 0);

    glfwSetWindowIcon(window, 3, windowIcon);

    stbi_image_free(windowIcon[0].pixels);
    stbi_image_free(windowIcon[1].pixels);
    stbi_image_free(windowIcon[2].pixels);

    // Loading opengl

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return 1;

    // Opengl setup
    glViewport(0, 0, 800, 800);

    // Enable show triangles
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup imgui

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::GetIO().IniFilename = "./data/imgui.ini";
    // ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Run the program
    voxelGrapher.emplace(window, defaultWidth, defaultHeight);
    voxelGrapher.value().run();

    // Turn off

    ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
  } catch (const VoxelGrapherError& e) {
    writeLogFile(e.msg);
    return 1;
  } catch (const std::exception& e) {
    writeLogFile(e.what());
    return 2;
  } catch (...) {
    writeLogFile("Unexpected error.");
    return 3;
  }

  return 0;
}

#ifdef _WIN32

#include <windows.h>

int APIENTRY WinMain(
  HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  return main();
}

#endif