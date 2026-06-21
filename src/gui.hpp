#pragma once

#include <string>
#include <vector>

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

  GUIparams(const std::string& blockFunc,
    const std::string& displayFunc);
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

  float renderScale = 1;

  void menuBar();

  void helpWindow();

  void infoWindow();

  void paramWindow();

  void functionWindow();

  void loadPopup();

  void savePopup();

public:
  GUI(GUIparams& params);

  GUI(const GUI&) = delete;
  GUI& operator=(const GUI&) = delete;

  void process(float time, float delta, bool cameraMode);

  void draw(float time, float delta) const;

  void resetFuncs();
};