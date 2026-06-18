#version 330 core

layout (location = 0) in vec2 attribPos;
layout (location = 1) in vec2 attribTexturePos;

out vec2 texturePos;

void main() {
  gl_Position = vec4(attribPos, 0, 1);
  texturePos = attribTexturePos;
}