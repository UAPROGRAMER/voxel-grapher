#include "gui.hpp"
#include "imgui.hpp"
#include "math.hpp"
#include "utils.hpp"

#include <string>
#include <filesystem>

GUIparams::GUIparams(const std::string& blockFunc,
  const std::string& displayFunc, std::vector<Variable>& variables)
  : blockFunc(blockFunc), displayFunc(displayFunc), variables(variables) {}

GUI::GUI(GUIparams& params) : params(params) {
  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  savefileNameText.resize(65);

  variableNameText.resize(65);

  resetFuncs();
}

void GUI::menuBar() {
  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("Windows")) {
    if (ImGui::MenuItem("Help", "Alt+Z")) isHelpWindow = !isHelpWindow;
    if (ImGui::MenuItem("Info", "Alt+X")) isInfoWindow = !isInfoWindow;
    if (ImGui::MenuItem("Parameters", "Alt+C")) isParamWindow = !isParamWindow;
    if (ImGui::MenuItem("Function", "Alt+V"))
      isFunctionWindow = !isFunctionWindow;
    if (ImGui::MenuItem("Variables", "Alt+B"))
      isVariablesWindow = !isVariablesWindow;
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Tools")) {
    if (ImGui::MenuItem("Screenshot")) params.screenshot = true;
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

void GUI::helpWindow() {
  if (isHelpWindow) {
    ImGui::Begin("Help", &isHelpWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted("Movement:");
    ImGui::TextUnformatted("");
    ImGui::TextUnformatted("E, Q - Up, Down");
    ImGui::TextUnformatted("W, S - Forwards, Backwards");
    ImGui::TextUnformatted("D, A - Right, Left");
    ImGui::TextUnformatted("Shift - 5 times slower");
    ImGui::TextUnformatted("Control - 5 times faster");
    ImGui::TextUnformatted("");
    ImGui::TextUnformatted("Function:");
    ImGui::TextUnformatted("");
    ImGui::TextUnformatted("Block Func:");
    ImGui::TextUnformatted("Must return an int, if 0 - continue");
    ImGui::TextUnformatted("Display Func:");
    ImGui::TextUnformatted("Must return vec4 for a pixel color");

    ImGui::End();
  }
}

void GUI::infoWindow() {
  if (isInfoWindow) {
    ImGui::Begin("Info", &isInfoWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);

    ImGui::Text("Pos: %.2f %.2f %.2f", params.posX, params.posY, params.posZ);

    ImGui::Text("Time: %.2f", params.time);

    ImGui::Text("Yaw: %.2f", params.yaw);

    ImGui::Text("Pitch: %.2f", params.pitch);

    ImGui::End();
  }
}

void GUI::paramWindow() {
  if (isParamWindow) {
    ImGui::Begin("Params", &isParamWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::DragFloat(
      "Max ray length", &params.maxRayLength, 1, 10, 500, "%.0f");

    if (ImGui::DragFloat(
          "Render scale", &renderScale, 0.01, 0.3, 1.5, "%.1f")) {
      params.renderScale = round(renderScale * 10) / 10;
      params.isRenderScaleChanged = true;
    }

    if (ImGui::Checkbox("Interpolation", &params.interpolateRender))
      params.interpolationChanged = true;

    ImGui::End();
  }
}

void GUI::functionWindow() {
  if (isFunctionWindow) {
    ImGui::Begin(
      "Function", &isFunctionWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted("Block Function:");

    ImGui::InputTextMultiline("##Block Func", blockFuncText.data(), 1024,
      ImVec2(ImGui::GetTextLineHeight() * 32, ImGui::GetTextLineHeight() * 14));

    ImGui::TextUnformatted("Display Function:");

    ImGui::InputTextMultiline("##Display Func", displayFuncText.data(), 1024,
      ImVec2(ImGui::GetTextLineHeight() * 32, ImGui::GetTextLineHeight() * 14));

    if (params.isFuncError)
      ImGui::TextColored(ImVec4(1, 0, 0, 1), params.funcError.c_str());

    if (ImGui::Button("Reset")) params.resetFuncs = true;

    ImGui::SameLine();

    if (ImGui::Button("Compile")) {
      params.blockFunc = std::string(blockFuncText.data());
      params.displayFunc = std::string(displayFuncText.data());

      params.compileFuncs = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Load")) isLoadPopup = true;

    ImGui::SameLine();

    if (ImGui::Button("Save")) isSavePopup = true;

    ImGui::End();
  }
}

void GUI::variablesWindow() {
  if (isVariablesWindow) {
    ImGui::Begin(
      "Variables", &isVariablesWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted("Variables:");

    if (ImGui::BeginListBox("##Variables",
          ImVec2(12 * ImGui::GetTextLineHeightWithSpacing(),
            8 * ImGui::GetTextLineHeightWithSpacing()))) {
      for (size_t i = 0; i < params.variables.size(); i++) {
        if (ImGui::Selectable(
              params.variables.at(i).name.c_str(), variableIndex == i))
          variableIndex = i;

        if (variableIndex == i) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }

    if (variableIndex != (size_t)-1)
      ImGui::DragFloat("##Value", &params.variables.at(variableIndex).value,
        params.variables.at(variableIndex).step,
        params.variables.at(variableIndex).min,
        params.variables.at(variableIndex).max);

    if (ImGui::Button("Add")) isAddVariablePopup = true;

    ImGui::SameLine();

    if (ImGui::Button("Change") && variableIndex != (size_t)-1)
      isChangeVariablePopup = true;

    ImGui::SameLine();

    if (ImGui::Button("Delete") && variableIndex != (size_t)-1) {
      params.variables.erase(params.variables.begin() + variableIndex);
      variableIndex = (size_t)-1;
    }

    ImGui::End();
  }
}

void GUI::loadPopup() {
  if (ImGui::BeginPopupModal(
        "Load", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Savefiles:");

    if (ImGui::BeginListBox("##Savefiles",
          ImVec2(12 * ImGui::GetTextLineHeightWithSpacing(),
            8 * ImGui::GetTextLineHeightWithSpacing()))) {
      for (size_t i = 0; i < savefilesC.size(); i++) {
        if (ImGui::Selectable(savefilesC.at(i), savefileIndex == i))
          savefileIndex = i;

        if (savefileIndex == i) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }

    if (ImGui::Button("Load") && savefileIndex != (size_t)-1) {
      params.savefile = savefiles.at(savefileIndex);
      params.isLoad = true;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete") && savefileIndex != (size_t)-1) {
      params.savefile = savefiles.at(savefileIndex);
      params.isDelete = true;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::savePopup() {
  if (ImGui::BeginPopupModal(
        "Save", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Name:");

    ImGui::InputText("##Name", savefileNameText.data(), 64);

    if (ImGui::Button("Save")) {
      params.savefile = std::string(savefileNameText.data());
      params.isSave = true;
      params.blockFunc = std::string(blockFuncText.data());
      params.displayFunc = std::string(displayFuncText.data());

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::addVariablePopup() {
  if (ImGui::BeginPopupModal(
        "Add Variable", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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
          for (const auto& variable : params.variables)
            if (variable.name == name) return false;
          
          return true;
        }(std::string(variableNameText.data()))) {
      params.variables.push_back(
        Variable(std::string(variableNameText.data())));

      params.variables.at(params.variables.size() - 1).step = variableStep;
      params.variables.at(params.variables.size() - 1).min = variableMin;
      params.variables.at(params.variables.size() - 1).max = variableMax;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::changeVariablePopup() {
  if (ImGui::BeginPopupModal(
        "Change Variable", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Step:");

    ImGui::InputFloat("##Step", &variableStep);

    ImGui::TextUnformatted("Min:");

    ImGui::InputFloat("##Min", &variableMin);

    ImGui::TextUnformatted("Max:");

    ImGui::InputFloat("##Max", &variableMax);

    if (ImGui::Button("Confirm")) {
      params.variables.at(variableIndex).step = variableStep;
      params.variables.at(variableIndex).min = variableMin;
      params.variables.at(variableIndex).max = variableMax;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void GUI::process(float time, float delta, bool cameraMode) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Shortcuts
  if (params.isHelpShortcut) {
    isHelpWindow = true;
    params.isHelpShortcut = false;
  }

  if (params.isInfoShortcut) {
    isInfoWindow = true;
    params.isInfoShortcut = false;
  }

  if (params.isParamShortcut) {
    isParamWindow = true;
    params.isParamShortcut = false;
  }

  if (params.isFunctionShortcut) {
    isFunctionWindow = true;
    params.isFunctionShortcut = false;
  }

  if (params.isVariablesShortcut) {
    isVariablesWindow = true;
    params.isVariablesShortcut = false;
  }

  // Main windows and other
  menuBar();

  infoWindow();

  if (!cameraMode) {
    helpWindow();
    paramWindow();
    functionWindow();
    variablesWindow();
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

    variableStep = params.variables.at(variableIndex).step;
    variableMin = params.variables.at(variableIndex).min;
    variableMax = params.variables.at(variableIndex).max;

    ImGui::OpenPopup("Change Variable");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
      ImGuiCond_Appearing, ImVec2(0.5, 0.5));
  }

  loadPopup();
  savePopup();
  addVariablePopup();
  changeVariablePopup();
}

void GUI::draw(float time, float delta) const {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::resetFuncs() {
  blockFuncText.clear();
  displayFuncText.clear();

  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  for (std::size_t i = 0; i < params.blockFunc.size(); i++)
    blockFuncText.at(i) = params.blockFunc.at(i);

  for (std::size_t i = 0; i < params.displayFunc.size(); i++)
    displayFuncText.at(i) = params.displayFunc.at(i);
}