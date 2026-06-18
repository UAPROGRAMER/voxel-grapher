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

  GUIparams(const std::string& blockFunc,
    const std::string& displayFunc);
};

class GUI {
private:
  GUIparams& params;

  bool isParamWindow = false;
  bool isInfoWindow = false;
  bool isFunctionWindow = false;

  std::vector<char> blockFuncText;
  std::vector<char> displayFuncText;

  float renderScale = 1;

  void menuBar();

  void infoWindow();

  void paramWindow();

  void functionWindow();

public:
  GUI(GUIparams& params);

  GUI(const GUI&) = delete;
  GUI& operator=(const GUI&) = delete;

  void process(float time, float delta, bool cameraMode);

  void draw(float time, float delta) const;

  void resetFuncs();
};