#include "../graphics/imgui.hpp"
#include "gui.hpp"

void GUI::functionsWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isFunctionsWindow) {
    ImGui::Begin("Function", &isFunctionsWindow,
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextUnformatted("Block Function:");

    ImGui::InputTextMultiline("##Block Func", blockFuncText.data(), 1024,
                              ImVec2(ImGui::GetTextLineHeight() * 32,
                                     ImGui::GetTextLineHeight() * 14));

    ImGui::TextUnformatted("Display Function:");

    ImGui::InputTextMultiline("##Display Func", displayFuncText.data(), 1024,
                              ImVec2(ImGui::GetTextLineHeight() * 32,
                                     ImGui::GetTextLineHeight() * 14));

    if (busIn.error) {
      errorText = busIn.errorText;
      ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", errorText.c_str());
    }

    if (ImGui::Button("Reset")) busOut.resetProgram = true;

    ImGui::SameLine();

    if (ImGui::Button("Compile")) {
      busOut.blockFunc = std::string(blockFuncText.data());
      busOut.displayFunc = std::string(displayFuncText.data());

      busOut.compileProgram = true;
    }

    ImGui::End();
  }
}