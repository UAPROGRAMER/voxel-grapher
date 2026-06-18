#version 330 core

in vec2 texturePos;

out vec4 endColor;

uniform sampler2D Texture;

void main() {
  endColor = texture(Texture, texturePos);
}