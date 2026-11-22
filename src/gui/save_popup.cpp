#include "../graphics/imgui.hpp"
#include "gui.hpp"

void GUI::savePopup(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (ImGui::BeginPopupModal("Save", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextUnformatted("Name:");

    ImGui::InputText("##Name", savefileNameText.data(), 64);

    if (ImGui::Button("Save")) {
      busOut.savefile = std::string(savefileNameText.data());
      busOut.saveSavefile = true;
      busOut.blockFunc = std::string(blockFuncText.data());
      busOut.displayFunc = std::string(displayFuncText.data());

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}