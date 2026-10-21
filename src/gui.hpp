#pragma once

#include <string>
#include <vector>

#include "variable.hpp"

struct GUIparams {
  float maxRayLength = 50;
  std::string blockFunc;
  std::string displayFunc;
  bool resetFuncs = false;
  bool compileFuncs = false;
  bool isFuncError = false;
  std::string funcError;
  float renderScale = 1;
  bool isRenderScaleChanged = false;
  bool interpolateRender = false;
  bool interpolationChanged = false;
  bool isLoad = false;
  bool isSave = false;
  bool isDelete = false;
  std::string savefile;
  float posX;
  float posY;
  float posZ;
  float time;
  bool isHelpShortcut = false;
  bool isParamShortcut = false;
  bool isInfoShortcut = false;
  bool isFunctionShortcut = false;
  bool isVariablesShortcut = false;
  bool screenshot = false;
  std::vector<Variable>& variables;
  float yaw;
  float pitch;
  float fov = 45;
  float sensitivity = 0.1;
  bool cameraChanged = false;
  bool isLoadShortcut = false;
  bool isSaveShortcut = false;

  GUIparams(const std::string& blockFunc, const std::string& displayFunc,
            std::vector<Variable>& variables);
};

class GUI {
 private:
  GUIparams& params;

  bool isHelpWindow = false;
  bool isParamWindow = false;
  bool isInfoWindow = false;

  bool isFunctionWindow = false;
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

  void menuBar();

  void helpWindow();

  void infoWindow();

  void paramWindow();

  void functionWindow();

  void variablesWindow();

  void loadPopup();

  void savePopup();

  void addVariablePopup();

  void changeVariablePopup();

 public:
  GUI(GUIparams& params);

  GUI(const GUI&) = delete;
  GUI& operator=(const GUI&) = delete;

  void process(float time, float delta, bool cameraMode);

  void draw(float time, float delta) const;

  void resetFuncs();
};