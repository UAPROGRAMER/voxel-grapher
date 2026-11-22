#include "../graphics/imgui.hpp"
#include "gui.hpp"
#include "imgui.h"

void GUI::teleportPopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Teleport", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::InputFloat3("##Pos", teleportPos);

    if (ImGui::Button("Teleport")) {
      busOut.teleport = true;
      busOut.teleportPos =
          glm::vec3(teleportPos[0], teleportPos[1], teleportPos[2]);

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}