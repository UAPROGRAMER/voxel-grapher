#include "gui.hpp"
#include "imgui.hpp"
#include "math.hpp"

#include <string>

GUIparams::GUIparams(
  const std::string& blockFunc, const std::string& displayFunc)
  : blockFunc(blockFunc), displayFunc(displayFunc) {}

GUI::GUI(GUIparams& params) : params(params) {
  blockFuncText.resize(1025);
  displayFuncText.resize(1025);

  resetFuncs();
}

void GUI::menuBar() {
  ImGui::BeginMainMenuBar();

  if (ImGui::BeginMenu("Windows")) {
    if (ImGui::MenuItem("Info")) isInfoWindow = !isInfoWindow;
    if (ImGui::MenuItem("Params")) isParamWindow = !isParamWindow;
    if (ImGui::MenuItem("Function")) isFunctionWindow = !isFunctionWindow;
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}

void GUI::infoWindow() {
  if (isInfoWindow) {
    ImGui::Begin("Info", &isInfoWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted(
      ("FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str());

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

    ImGui::Text("Block Func:");

    ImGui::InputTextMultiline("##Block Func", blockFuncText.data(), 1024,
      ImVec2(ImGui::GetTextLineHeight() * 32, ImGui::GetTextLineHeight() * 14));

    ImGui::Text("Display Func:");

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

    ImGui::End();
  }
}

void GUI::process(float time, float delta, bool cameraMode) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  menuBar();

  if (!cameraMode) {
    infoWindow();
    paramWindow();
    functionWindow();
  }
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