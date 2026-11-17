#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "glm/ext/vector_float3.hpp"
#include "variable.hpp"

struct GUIBusIn {
  bool cameraMode = false;
  glm::vec3 cameraPos = glm::vec3();
  float time = 0;
  float yaw = 0;
  float pitch = 0;
  float fov = 0;
  float sensitivity = 0;

  float maxRayLength = 0;

  float renderScale = 0;
  bool interpolation = false;

  bool error = false;
  std::string_view errorText = "";

  const std::vector<Variable>& variables;

  bool openHelp = false;
  bool openInfo = false;
  bool openParams = false;
  bool openFunctions = false;
  bool openVariables = false;
  bool openSave = false;
  bool openLoad = false;

  GUIBusIn(const std::vector<Variable>& variables);
};

struct GUIBusOut {
  bool takeScreenshot = false;
  
  float maxRayLength = 0;

  bool renderScaleChanged = false;
  float renderScale = 0;

  bool interpolationChanged = false;
  bool interpolation = false;

  bool cameraChanged = false;
  float fov = 0;
  float sensitivity = 0;

  bool resetProgram = false;
  bool compileProgram = false;
  std::string blockFunc = "";
  std::string displayFunc = "";

  bool variableValueChanged = false;
  bool variableChanged = false;
  bool variableDeleted = false;
  bool variableAdded = false;
  size_t variableIndex = 0;
  std::string variableName = "";
  float variableValue = 0;
  float variableStep = 0;
  float variableMin = 0;
  float variableMax = 0;

  bool loadSavefile = false;
  bool saveSavefile = false;
  bool deleteSavefile = false;
  std::string savefile = "";
};

class GUI {
 private:
  bool isHelpWindow = false;

  bool isParamWindow = false;
  
  bool isInfoWindow = false;

  bool isFunctionsWindow = false;
  std::string errorText;
  std::vector<char> blockFuncText;
  std::vector<char> displayFuncText;

  bool isLoadPopup = false;
  std::vector<std::string> savefiles;
  std::vector<const char*> savefilesC;
  size_t savefileIndex;

  bool isSavePopup = false;
  std::vector<char> savefileNameText;

  bool isVariablesWindow = false;
  size_t variableIndex = (size_t)-1;

  bool isAddVariablePopup = false;
  std::vector<char> variableNameText;

  bool isChangeVariablePopup = false;
  float variableStep = 0.01;
  float variableMin = 0;
  float variableMax = 0;

  float renderScale = 1;

  void menuBar(const GUIBusIn& busIn, GUIBusOut& busOut);

  void helpWindow(const GUIBusIn& busIn, GUIBusOut& busOut);

  void infoWindow(const GUIBusIn& busIn, GUIBusOut& busOut);

  void paramWindow(const GUIBusIn& busIn, GUIBusOut& busOut);

  void functionWindow(const GUIBusIn& busIn, GUIBusOut& busOut);

  void variablesWindow(const GUIBusIn& busIn, GUIBusOut& busOut);

  void loadPopup(const GUIBusIn& busIn, GUIBusOut& busOut);

  void savePopup(const GUIBusIn& busIn, GUIBusOut& busOut);

  void addVariablePopup(const GUIBusIn& busIn, GUIBusOut& busOut);

  void changeVariablePopup(const GUIBusIn& busIn, GUIBusOut& busOut);

 public:
  GUI(std::string_view blockFunc, std::string_view displayFunc);

  GUI(const GUI&) = delete;
  GUI& operator=(const GUI&) = delete;

  GUIBusOut process(const GUIBusIn& bus);

  void draw() const;

  void resetFuncs(std::string_view blockFunc, std::string_view displayFunc);
};