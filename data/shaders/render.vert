#version 330 core

layout(location = 0) in vec2 attribPos;

out vec2 fragPos;

void main() {
  gl_Position = vec4(attribPos, 0, 1);
  fragPos = attribPos;
};