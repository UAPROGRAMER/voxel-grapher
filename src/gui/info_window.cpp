#include "../graphics/imgui.hpp"
#include "gui.hpp"

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