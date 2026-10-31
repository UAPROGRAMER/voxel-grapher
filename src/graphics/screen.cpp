#include "screen.hpp"

#include "opengl.hpp"

float displayVertices[] = {
    // clang-format off

    // positions
    -1, -1, 0, 0,     // Bottom Left
    -1, 1,  0, 1,     // Top Left
    1, 1, 1, 1,   // Top Right
    1, -1, 1, 0  // Bottom Right

    // clang-format on
};

unsigned int displayIndices[] = {
    2, 0, 3,  // Bottom Right
    2, 1, 0   // Top Left
};

Screen::Screen(int _renWidth, int _renHeight, int _realWidth, int _realHeight)
    : renWidth(_renWidth),
      renHeight(_renHeight),
      realWidth(_realWidth),
      realHeight(_realHeight) {
  // Texture

  glGenTextures(1, &texture);

  changeSize(renWidth, renHeight, realWidth, realHeight);

  setFilterNearest();

  glBindTexture(GL_TEXTURE_2D, 0);

  // Framebuffer

  glGenFramebuffers(1, &FBO);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw 999;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Rect

  glGenVertexArrays(1, &VAO);

  glGenBuffers(1, &VBO);

  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(displayVertices), displayVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(displayIndices), displayIndices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

Screen::~Screen() {
  glDeleteFramebuffers(1, &FBO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Screen::setFilterLinear() {
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Screen::setFilterNearest() {
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Screen::changeSize(int _renWidth, int _renHeight, int _realWidth,
                        int _realHeight) {
  renWidth = _renWidth;
  renHeight = _renHeight;
  realWidth = _realWidth;
  realHeight = _realHeight;

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renWidth, renHeight, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Screen::render() const {
  glViewport(0, 0, renWidth, renHeight);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, realWidth, realHeight);
}

void Screen::draw() const {
  glBindTexture(GL_TEXTURE_2D, texture);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);

  glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<uint8_t> Screen::getPixels() {
  std::vector<uint8_t> screenshotRGBA;
  screenshotRGBA.resize(renWidth * renHeight * 4ull);

  glBindTexture(GL_TEXTURE_2D, texture);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                screenshotRGBA.data());

  glBindTexture(GL_TEXTURE_2D, 0);

  std::vector<uint8_t> screenshotBGR;
  screenshotBGR.resize(renWidth * renHeight * 3);

  for (size_t i = 0; i < renWidth * renHeight; i++) {
    screenshotBGR.at(i * 3) = screenshotRGBA.at(i * 4 + 2);
    screenshotBGR.at(i * 3 + 1) = screenshotRGBA.at(i * 4 + 1);
    screenshotBGR.at(i * 3 + 2) = screenshotRGBA.at(i * 4);
  }

  return screenshotBGR;
}