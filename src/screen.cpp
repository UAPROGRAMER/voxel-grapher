#define GLFW_INCLUDE_NONE

#include "screen.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "error.hpp"

float displayVertices[] = {
    // clang-format off

    // positions
    -1, -1, 0, 0,     // Bottom Left
    -1, 1,  0, 1,     // Top Left
    1,  1,  1, 1,   // Top Right
    1,  -1, 1, 0  // Bottom Right

    // clang-format on
};

unsigned int displayIndices[] = {
    2, 0, 3,  // Bottom Right
    2, 1, 0   // Top Left
};

Screen::Screen(unsigned int& renderTexture, int width, int height)
    : width(width), height(height) {
  // Framebuffer texture

  glGenTextures(1, &renderTexture);

  glBindTexture(GL_TEXTURE_2D, renderTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);

  // Framebuffer

  glGenFramebuffers(1, &FBO);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         renderTexture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw VoxelGrapherError("Failed to create framebuffer.");

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

void Screen::render() const {
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Screen::draw() const {
  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}