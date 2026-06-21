#include "gui.hpp"
#include "imgui.hpp"
#include "math.hpp"

#include <string>
#include <filesystem>

GUIparams::GUIparams(
  const std::string& blockFunc, const std::string& displayFunc)
  : blockFunc(blockFunc), displayFunc(displayFunc) {}

GUI::GUI(GUIparams& params) : params(params) {
  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  savefileNameText.resize(65);

  resetFuncs();
}

void GUI::menuBar() {
  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("Windows")) {
    if (ImGui::MenuItem("Help")) isHelpWindow = !isHelpWindow;
    if (ImGui::MenuItem("Info")) isInfoWindow = !isInfoWindow;
    if (ImGui::MenuItem("Params")) isParamWindow = !isParamWindow;
    if (ImGui::MenuItem("Function")) isFunctionWindow = !isFunctionWindow;
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

void GUI::process(float time, float delta, bool cameraMode) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  menuBar();

  infoWindow();

  if (!cameraMode) {
    helpWindow();
    paramWindow();
    functionWindow();
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

  loadPopup();
  savePopup();
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