#pragma once

#include <cstdint>
#include <vector>

class Screen {
 private:
  int renWidth;
  int renHeight;
  int realWidth;
  int realHeight;
  unsigned FBO;
  unsigned VAO;
  unsigned VBO;
  unsigned EBO;
  unsigned texture;

 public:
  Screen(int renWidth, int renHeight, int realWidth, int realHeight);

  ~Screen();

  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  void setFilterLinear();

  void setFilterNearest();

  void changeSize(int renWidth, int renHeight, int realWidth, int realHeight);

  void render() const;

  void draw() const;

  std::vector<uint8_t> getPixels();
};