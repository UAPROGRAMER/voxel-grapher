#include "../graphics/imgui.hpp"
#include "gui.hpp"

void GUI::menuBar(const GUIBusIn& busIn, GUIBusOut& busOut) {
  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("Windows")) {
    if (ImGui::MenuItem("Help", "Alt+H", isHelpWindow))
      isHelpWindow = !isHelpWindow;
    if (ImGui::MenuItem("Info", "Alt+I", isInfoWindow))
      isInfoWindow = !isInfoWindow;
    if (ImGui::MenuItem("Parameters", "Alt+P", isParamWindow))
      isParamWindow = !isParamWindow;
    if (ImGui::MenuItem("Functions", "Alt+F", isFunctionsWindow))
      isFunctionsWindow = !isFunctionsWindow;
    if (ImGui::MenuItem("Variables", "Alt+V", isVariablesWindow))
      isVariablesWindow = !isVariablesWindow;
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Tools")) {
    if (ImGui::BeginMenu("Screenshot")) {
      if (ImGui::MenuItem("png", "F2")) {
        busOut.takeScreenshot = true;
        busOut.screenshotFormat = ImageFormat::PNG;
      }
      if (ImGui::MenuItem("jpg")) {
        busOut.takeScreenshot = true;
        busOut.screenshotFormat = ImageFormat::JPG;
      }
      if (ImGui::MenuItem("bmp")) {
        busOut.takeScreenshot = true;
        busOut.screenshotFormat = ImageFormat::BMP;
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Teleport")) {
      if (ImGui::MenuItem("Center", "Alt+C")) {
        busOut.teleport = true;
        busOut.teleportPos = glm::vec3(0);
      }

      if (ImGui::MenuItem("Custom", "Alt+T")) isTeleportPopup = true;

      ImGui::EndMenu();
    }

    if (ImGui::MenuItem("Save", "Alt+S")) isSavePopup = true;
    if (ImGui::MenuItem("Load", "Alt+L")) isLoadPopup = true;
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Theme")) {
    if (ImGui::MenuItem("Classic", "Alt+1", busIn.theme == Theme::CLASSIC)) {
      if (busIn.theme != Theme::CLASSIC) {
        busOut.themeChanged = true;
        busOut.theme = Theme::CLASSIC;
      }
    } else if (ImGui::MenuItem("Light", "Alt+2", busIn.theme == Theme::LIGHT)) {
      if (busIn.theme != Theme::LIGHT) {
        busOut.themeChanged = true;
        busOut.theme = Theme::LIGHT;
      }
    } else if (ImGui::MenuItem("Dark", "Alt+3", busIn.theme == Theme::DARK)) {
      if (busIn.theme != Theme::DARK) {
        busOut.themeChanged = true;
        busOut.theme = Theme::DARK;
      }
    }
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}