#include "../graphics/imgui.hpp"
#include "gui.hpp"

void GUI::loadPopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Load", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Savefiles:");

    if (ImGui::BeginListBox(
            "##Savefiles", ImVec2(12 * ImGui::GetTextLineHeightWithSpacing(),
                                  8 * ImGui::GetTextLineHeightWithSpacing()))) {
      for (size_t i = 0; i < savefilesC.size(); i++) {
        if (ImGui::Selectable(savefilesC.at(i), savefileIndex == i))
          savefileIndex = i;

        if (savefileIndex == i) ImGui::SetItemDefaultFocus();
      }

      ImGui::EndListBox();
    }

    if (ImGui::Button("Load") && savefileIndex != (size_t)-1) {
      busOut.savefile = savefiles.at(savefileIndex);
      busOut.loadSavefile = true;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete") && savefileIndex != (size_t)-1) {
      busOut.savefile = savefiles.at(savefileIndex);
      busOut.deleteSavefile = true;

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}