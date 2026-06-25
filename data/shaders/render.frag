#version 330 core

// a very small number
const float epsilon = 0.000000001;
// math constants to the tenth digit
const float pi = 3.1415926535;
const float e = 2.7182818284;
const float phi = 1.6180339887;

in vec2 fragPos;

out vec4 endColor;

// Window
uniform int width;
uniform int height;

// Camera
uniform vec3 cameraFront;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform vec3 cameraPos;
uniform float cameraFov;

// Ray
uniform float maxRayLength;

// Time variable
uniform float t;

// Custom variables
@v

// Funcs

bool _stop = false;
void stop() { _stop = true; }

bool approx(float a, float b, float c) { return abs(a - b) <= c; }

int blockFunction(ivec3 coords, ivec3 rayDirSign) {
  float x = coords.x;
  float y = coords.y;
  float z = coords.z;
  @b  // For block func
}

vec4 displayFunction(float dist, int lastCoord, int blockValue, ivec3 coords) {
  float x = coords.x;
  float y = coords.y;
  float z = coords.z;
  @d  // For display func
}

void main() {
  int minSize = min(width, height);

  float pixelSize = 1.0 / minSize;

  // Remaping default opengl coords [-1;1] to screen size coords (camera)
  // smaller side of the window is 1 in length [-0.5;0.5] and larger is
  // proportianate
  vec2 pixelPos = vec2(width, height) * 0.5 * fragPos * pixelSize;

  // Pixel angle scale (pinhole effect)
  float scale = 2.0 * tan(radians(cameraFov) * 0.5);

  // Directional vector if the ray
  vec3 rayDir = normalize(cameraFront + pixelPos.x * scale * cameraRight +
    pixelPos.y * scale * cameraUp);

  // Adding epsilon to ensure no cordinate is 0
  rayDir += vec3(epsilon);

  // Inverse of the dir

  ivec3 rayDirSign = ivec3(sign(rayDir));

  vec3 rayDirInvAbs = abs(1.0 / rayDir);

  // Computing stuff

  endColor = vec4(1);

  ivec3 voxelCoord = ivec3(round(cameraPos));
  float rayLength = 0;

  // Nearest plane
  vec3 planePos = vec3(voxelCoord) + 0.5 * vec3(rayDirSign);

  // Steps to nearest plane
  vec3 t = (planePos - cameraPos) / rayDir;

  int lastCoord = 0;

  while (rayLength <= maxRayLength) {
    int blockValue = blockFunction(voxelCoord, rayDirSign);

    if (_stop) return;

    if (bool(blockValue)) {
      endColor = displayFunction(rayLength, lastCoord, blockValue, voxelCoord);
      return;
    }

    if (t.x < t.y) {
      if (t.x < t.z) {  // x is the closest
        voxelCoord.x += rayDirSign.x;
        rayLength = t.x;
        t.x += rayDirInvAbs.x;
        lastCoord = 1;
      } else {  // z is the closest
        voxelCoord.z += rayDirSign.z;
        rayLength = t.z;
        t.z += rayDirInvAbs.z;
        lastCoord = 3;
      }
    } else {
      if (t.y < t.z) {  // y is the closest
        voxelCoord.y += rayDirSign.y;
        rayLength = t.y;
        t.y += rayDirInvAbs.y;
        lastCoord = 2;
      } else {  // z is the closest
        voxelCoord.z += rayDirSign.z;
        rayLength = t.z;
        t.z += rayDirInvAbs.z;
        lastCoord = 3;
      }
    }
  }
};