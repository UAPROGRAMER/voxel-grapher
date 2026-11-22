#include <algorithm>
#include <string_view>

#include "../common/utils.hpp"
#include "../graphics/imgui.hpp"
#include "gui.hpp"

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
        std::find(busIn.variables.begin(), busIn.variables.end(),
                  Variable(std::string_view(variableNameText.data()))) ==
            busIn.variables.end()) {
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