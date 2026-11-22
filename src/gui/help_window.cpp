#include "../graphics/imgui.hpp"
#include "gui.hpp"

void GUI::helpWindow(const GUIBusIn& busIn, GUIBusOut& busOut) {
  if (isHelpWindow) {
    ImGui::Begin("Help", &isHelpWindow, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::TreeNode("Movement")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("E/Q      ->  move up/down");
      ImGui::TextUnformatted("W/S      ->  move forwards/backwards");
      ImGui::TextUnformatted("D/A      ->  move right/left");
      ImGui::TextUnformatted("Shift    ->  move 5 times slower");
      ImGui::TextUnformatted("Control  ->  move 5 times faster");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Windows")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Help (Alt+H)        ->  this window");
      ImGui::TextUnformatted(
          "Info (Alt+I)        ->  info about pos/fps/other");
      ImGui::TextUnformatted(
          "Parameters (Alt+P)  ->  parameters of the render");
      ImGui::TextUnformatted("Functions (Alt+F)   ->  edit functions");
      ImGui::TextUnformatted(
          "Variables (Alt+V)   ->  add/delete/change variables");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Tools")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted(
          "Screenshot  ->  take a screenshot in chosen format");
      ImGui::TextUnformatted("Save        ->  saves functions and variables");
      ImGui::TextUnformatted("Load        ->  loades functions and variables");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Theme")) {
      ImGui::TextUnformatted("");
      ImGui::TextUnformatted("Existing themes: Classic, Light, Dark.");
      ImGui::TextUnformatted("After changing theme the app will close.");
      ImGui::TextUnformatted("");

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Functions")) {
      if (ImGui::TreeNode("Block Function")) {
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("Used as a function for building the graph.");
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("Returns int");
        ImGui::TextUnformatted(
            "The value is then passed into the display function");
        ImGui::TextUnformatted("If the value is 0, calculation continues");
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("Variables that can used:");
        ImGui::TextUnformatted("ivec3 coords      ->  coords of the voxel");
        ImGui::TextUnformatted("float x, y, z     ->  short for coords.x/y/z");
        ImGui::TextUnformatted(
            "ivec3 rayDirSign  ->  sign for every direction (1/-1)");
        ImGui::TextUnformatted("");

        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Display Function")) {
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted(
            "Used for deciding what pixel to put on the screen.");
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("Returns vec4 for color of the pixel");
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("Variables that can used:");
        ImGui::TextUnformatted("ivec3 coords      ->  coords of the voxel");
        ImGui::TextUnformatted("float x, y, z     ->  short for coords.x/y/z");
        ImGui::TextUnformatted("float dist        ->  distance traveled");
        ImGui::TextUnformatted("int lastCoords    ->  last axis crossed");
        ImGui::TextUnformatted("    0  ->  inside a voxel");
        ImGui::TextUnformatted("    1  ->  ox");
        ImGui::TextUnformatted("    2  ->  oy");
        ImGui::TextUnformatted("    3  ->  oz");
        ImGui::TextUnformatted(
            "int blockValue    -> value returned by Block function");
        ImGui::TextUnformatted("");

        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Constants and Variables")) {
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("float epsilon  ->  1e-9");
        ImGui::TextUnformatted("float pi       ->  to the tenth digit");
        ImGui::TextUnformatted("float e        ->  to the tenth digit");
        ImGui::TextUnformatted("float phi      ->  to the tenth digit");
        ImGui::TextUnformatted("float t        ->  current time");
        ImGui::TextUnformatted("");

        ImGui::TreePop();
      }

      if (ImGui::TreeNode("Helper Functions")) {
        ImGui::TextUnformatted("");
        ImGui::TextUnformatted("bool approx(float a, float b, float c)");
        ImGui::TextUnformatted("");

        ImGui::TreePop();
      }

      ImGui::TreePop();
    }

    ImGui::End();
  }
}