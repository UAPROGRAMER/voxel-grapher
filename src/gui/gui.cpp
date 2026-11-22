#include "gui.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <string_view>

#include "../common/theme.hpp"
#include "../common/utils.hpp"
#include "../graphics/glm.hpp"
#include "../graphics/image.hpp"
#include "../graphics/imgui.hpp"

GUIBusIn::GUIBusIn(const std::vector<Variable>& variables)
    : variables(variables) {}

GUI::GUI(std::string_view blockFunc, std::string_view displayFunc) {
  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  savefileNameText.resize(65);

  variableNameText.resize(65);

  resetFuncs(blockFunc, displayFunc);
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
    paramsWindow(busIn, busOut);
    functionsWindow(busIn, busOut);
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