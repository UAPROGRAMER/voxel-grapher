#include "gui.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <string_view>

#include "common/theme.hpp"
#include "common/utils.hpp"
#include "graphics/glm.hpp"
#include "graphics/image.hpp"
#include "graphics/imgui.hpp"
#include "imgui.h"

GUIBusIn::GUIBusIn(const std::vector<Variable>& variables)
    : variables(variables) {}

GUI::GUI(std::string_view blockFunc, std::string_view displayFunc) {
  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  savefileNameText.resize(65);

  variableNameText.resize(65);

  resetFuncs(blockFunc, displayFunc);
}

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

void GUI::helpWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isHelpWindow) {
    ImGui::Begin("Help", &isHelpWindow, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::TreeNode("Movement")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("E/Q      ->  move up/down");
      ImGui::TextUnformatted("W/S      ->  move forwards/backwards");
      ImGui::TextUnformatted("D/A      ->  move right/left");
      ImGui::TextUnformatted("Shift    ->  move 5 times slower");
      ImGui::TextUnformatted("Control  ->  move 5 times faster");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Windows")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Help (Alt+H)        ->  this window");
      ImGui::TextUnformatted(
          "Info (Alt+I)        ->  info about pos/fps/other");
      ImGui::TextUnformatted(
          "Parameters (Alt+P)  ->  parameters of the render");
      ImGui::TextUnformatted("Functions (Alt+F)   ->  edit functions");
      ImGui::TextUnformatted(
          "Variables (Alt+V)   ->  add/delete/change variables");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Tools")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Screenshot  ->  take a screenshot as bmp");
      ImGui::TextUnformatted("Save        ->  saves functions and variables");
      ImGui::TextUnformatted("Load        ->  loades functions and variables");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Theme")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Existing themes: Classic, Light, Dark.");
      ImGui::TextUnformatted("After changing theme the app will close.");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    ImGui::End();
  }
}

void GUI::infoWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isInfoWindow) {
    ImGui::Begin("Info", &isInfoWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);

    ImGui::Text("Pos: %.2f %.2f %.2f", busIn.cameraPos.x, busIn.cameraPos.y,
                busIn.cameraPos.z);

    ImGui::Text("Time: %.2f", busIn.time);

    ImGui::Text("Yaw: %.2f", busIn.yaw);

    ImGui::Text("Pitch: %.2f", busIn.pitch);

    ImGui::End();
  }
}

void GUI::paramWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isParamWindow) {
    ImGui::Begin("Params", &isParamWindow, ImGuiWindowFlags_AlwaysAutoResize);

    busOut.maxRayLength = busIn.maxRayLength;
    ImGui::DragFloat("Max Ray Length", &busOut.maxRayLength, 1, 10, 1000,
                     "%.0f");

    busOut.renderScale = busIn.renderScale;
    if (ImGui::DragFloat("Render Scale", &renderScale, 0.005, 0.2, 2, "%.1f")) {
      busOut.renderScale = round(renderScale * 10) / 10;
      busOut.renderScaleChanged = true;
    }

    busOut.fov = busIn.fov;
    if (ImGui::DragFloat("Camera Fov", &busOut.fov, 1, 5, 180, "%.0f"))
      busOut.cameraChanged = true;

    busOut.sensitivity = busIn.sensitivity;
    if (ImGui::DragFloat("Camera Sensitivity", &busOut.sensitivity, 0.01, 0.01,
                         0.2, "%.2f"))
      busOut.cameraChanged = true;

    busOut.interpolation = busIn.interpolation;
    if (ImGui::Checkbox("Interpolation", &busOut.interpolation))
      busOut.interpolationChanged = true;

    ImGui::End();
  }
}

void GUI::functionWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isFunctionsWindow) {
    ImGui::Begin("Function", &isFunctionsWindow,
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted("Block Function:");

    ImGui::InputTextMultiline("##Block Func", blockFuncText.data(), 1024,
                              ImVec2(ImGui::GetTextLineHeight() * 32,
                                     ImGui::GetTextLineHeight() * 14));

    ImGui::TextUnformatted("Display Function:");

    ImGui::InputTextMultiline("##Display Func", displayFuncText.data(), 1024,
                              ImVec2(ImGui::GetTextLineHeight() * 32,
                                     ImGui::GetTextLineHeight() * 14));

    if (busIn.error) {
      errorText = busIn.errorText;
      ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", errorText.c_str());
    }

    if (ImGui::Button("Reset")) busOut.resetProgram = true;

    ImGui::SameLine();

    if (ImGui::Button("Compile")) {
      busOut.blockFunc = std::string(blockFuncText.data());
      busOut.displayFunc = std::string(displayFuncText.data());

      busOut.compileProgram = true;
    }

    ImGui::End();
  }
}

void GUI::variablesWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isVariablesWindow) {
    ImGui::Begin("Variables", &isVariablesWindow,
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted("Variables:");

    if (ImGui::BeginListBox(
            "##Variables", ImVec2(12 * ImGui::GetTextLineHeightWithSpacing(),
                                  8 * ImGui::GetTextLineHeightWithSpacing()))) {
      for (size_t i = 0; i < busIn.variables.size(); i++) {
        if (ImGui::Selectable(busIn.variables.at(i).name.c_str(),
                              variableIndex == i))
          variableIndex = i;

        if (variableIndex == i) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }

    if (variableIndex != (size_t)-1) {
      busOut.variableValueChanged = true;
      busOut.variableIndex = variableIndex;
      busOut.variableValue = busIn.variables.at(variableIndex).value;

      ImGui::DragFloat("##Value", &busOut.variableValue,
                       busIn.variables.at(variableIndex).step,
                       busIn.variables.at(variableIndex).min,
                       busIn.variables.at(variableIndex).max);
    }

    if (ImGui::Button("Add")) isAddVariablePopup = true;

    ImGui::SameLine();

    if (ImGui::Button("Change") && variableIndex != (size_t)-1)
      isChangeVariablePopup = true;

    ImGui::SameLine();

    if (ImGui::Button("Delete") && variableIndex != (size_t)-1) {
      busOut.variableDeleted = true;
      busOut.variableIndex = variableIndex;
      variableIndex = (size_t)-1;
    }

    ImGui::End();
  }
}

void GUI::loadPopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Load", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Savefiles:");

    if (ImGui::BeginListBox(
            "##Savefiles", ImVec2(12 * ImGui::GetTextLineHeightWithSpacing(),
                                  8 * ImGui::GetTextLineHeightWithSpacing()))) {
      for (size_t i = 0; i < savefilesC.size(); i++) {
        if (ImGui::Selectable(savefilesC.at(i), savefileIndex == i))
          savefileIndex = i;

        if (savefileIndex == i) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }

    if (ImGui::Button("Load") && savefileIndex != (size_t)-1) {
      busOut.savefile = savefiles.at(savefileIndex);
      busOut.loadSavefile = true;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete") && savefileIndex != (size_t)-1) {
      busOut.savefile = savefiles.at(savefileIndex);
      busOut.deleteSavefile = true;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::savePopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Save", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Name:");

    ImGui::InputText("##Name", savefileNameText.data(), 64);

    if (ImGui::Button("Save")) {
      busOut.savefile = std::string(savefileNameText.data());
      busOut.saveSavefile = true;
      busOut.blockFunc = std::string(blockFuncText.data());
      busOut.displayFunc = std::string(displayFuncText.data());

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::addVariablePopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Add Variable", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Name:");

    ImGui::InputText("##Name", variableNameText.data(), 64);

    ImGui::TextUnformatted("Step:");

    ImGui::InputFloat("##Step", &variableStep);

    ImGui::TextUnformatted("Min:");

    ImGui::InputFloat("##Min", &variableMin);

    ImGui::TextUnformatted("Max:");

    ImGui::InputFloat("##Max", &variableMax);

    if (ImGui::Button("Add") &&
            isValidId(std::string(variableNameText.data())) &&
            [&](const std::string& name) -> bool {
          for (const auto& variable : busIn.variables)
            if (variable.name == name) return false;

          return true;
        }(std::string(variableNameText.data()))) {
      busOut.variableAdded = true;

      busOut.variableName = std::string(variableNameText.data());
      busOut.variableStep = variableStep;
      busOut.variableMin = variableMin;
      busOut.variableMax = variableMax;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::changeVariablePopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Change Variable", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Step:");

    ImGui::InputFloat("##Step", &variableStep);

    ImGui::TextUnformatted("Min:");

    ImGui::InputFloat("##Min", &variableMin);

    ImGui::TextUnformatted("Max:");

    ImGui::InputFloat("##Max", &variableMax);

    if (ImGui::Button("Confirm")) {
      busOut.variableChanged = true;

      busOut.variableStep = variableStep;
      busOut.variableMin = variableMin;
      busOut.variableMax = variableMax;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

GUIBusOut GUI::process(const GUIBusIn& busIn) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  GUIBusOut busOut;

  busOut.maxRayLength = busIn.maxRayLength;
  busOut.renderScale = busIn.renderScale;
  busOut.interpolation = busIn.interpolation;

  // Shortcuts
  if (busIn.openHelp) isHelpWindow = true;

  if (busIn.openInfo) isInfoWindow = true;

  if (busIn.openParams) isParamWindow = true;

  if (busIn.openFunctions) isFunctionsWindow = true;

  if (busIn.openVariables) isVariablesWindow = true;

  if (busIn.openSave) isSavePopup = true;

  if (busIn.openLoad) isLoadPopup = true;

  // Main windows and other
  menuBar(busIn, busOut);

  infoWindow(busIn, busOut);

  if (!busIn.cameraMode) {
    helpWindow(busIn, busOut);
    paramWindow(busIn, busOut);
    functionWindow(busIn, busOut);
    variablesWindow(busIn, busOut);

    // if (ImGui::Begin("Style editor")) {
    //   ImGui::ShowStyleEditor();
    //   ImGui::End();
    // }
  }

  if (isLoadPopup) {
    isLoadPopup = false;
    savefiles.clear();
    savefilesC.clear();
    savefileIndex = (size_t)-1;

    for (const auto& file : std::filesystem::directory_iterator("./data/saves"))
      if (!std::filesystem::is_regular_file(file))
        savefiles.push_back(file.path().stem().string());

    std::sort(savefiles.begin(), savefiles.end());

    savefilesC.reserve(savefiles.size());

    for (const auto& file : savefiles) savefilesC.push_back(file.c_str());

    ImGui::OpenPopup("Load");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                            ImGuiCond_Appearing, ImVec2(0.5, 0.5));
  }

  if (isSavePopup) {
    isSavePopup = false;

    savefileNameText.clear();
    savefileNameText.resize(65);

    ImGui::OpenPopup("Save");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                            ImGuiCond_Appearing, ImVec2(0.5, 0.5));
  }

  if (isAddVariablePopup) {
    isAddVariablePopup = false;

    variableNameText.clear();
    variableNameText.resize(65);

    variableStep = 0.01;
    variableMin = 0;
    variableMax = 1;

    ImGui::OpenPopup("Add Variable");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                            ImGuiCond_Appearing, ImVec2(0.5, 0.5));
  }

  if (isChangeVariablePopup) {
    isChangeVariablePopup = false;

    variableStep = busIn.variables.at(variableIndex).step;
    variableMin = busIn.variables.at(variableIndex).min;
    variableMax = busIn.variables.at(variableIndex).max;

    ImGui::OpenPopup("Change Variable");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                            ImGuiCond_Appearing, ImVec2(0.5, 0.5));
  }

  loadPopup(busIn, busOut);
  savePopup(busIn, busOut);
  addVariablePopup(busIn, busOut);
  changeVariablePopup(busIn, busOut);

  return busOut;
}

void GUI::draw() const {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::resetFuncs(std::string_view blockFunc, std::string_view displayFunc) {
  blockFuncText.clear();
  displayFuncText.clear();

  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  for (std::size_t i = 0; i < blockFunc.size(); i++)
    blockFuncText.at(i) = blockFunc.at(i);

  for (std::size_t i = 0; i < displayFunc.size(); i++)
    displayFuncText.at(i) = displayFunc.at(i);

  variableIndex = (size_t)-1;
}