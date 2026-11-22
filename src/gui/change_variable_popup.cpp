#include "../graphics/imgui.hpp"
#include "gui.hpp"

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