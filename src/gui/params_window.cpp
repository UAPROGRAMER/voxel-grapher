#include "../graphics/imgui.hpp"
#include "gui.hpp"

void GUI::paramsWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
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