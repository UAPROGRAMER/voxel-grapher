#pragma once

class Screen {
 private:
  int width;
  int height;
  unsigned int FBO;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

 public:
  Screen(unsigned int& renderTexture, int width, int height);

  ~Screen();

  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  void render() const;

  void draw() const;
};