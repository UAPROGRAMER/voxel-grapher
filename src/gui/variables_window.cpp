#include "../graphics/imgui.hpp"
#include "gui.hpp"

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