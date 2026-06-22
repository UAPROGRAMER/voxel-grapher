# Voxel Grapher

Cross platform program for visualizing 3D graphs, both using math and programing. The rendering of the voxels is fully done on gpu, per pixel. This gives results as close to perfect as float point will allow.

## About the project

### Capabilities

- Compile and render custom graphs on the GPU during runtime
- Save and load custom graphs
- Diferent types of voxels that you can create
- Custom display function for the voxels

### Used

- C++ is used as a main language
- GLFW is used as a cross platform window manager
- OpenGL is used as a rendering library
- Glad is used as a loader for OpenGL
- GLSL is used as a shader language and as a language for functions
- GLM is used as a math library
- ImGui is used as a GUI libary
- mINI is used as a .ini file reading library
- stb_image is used as a image loading library (part of stb library)

### Step by step

All voxel and function calculations are done directly on GPU via raycasting.
For every pixel on a custom texture, ray is sent and function is computed for every voxel ray goes through.
If `blockFunction` in shader returns non 0 value for curent voxel, pixel is colored according to `displayFunction` else ray continues.
If ray length exceeds `maxRayLength`, pixel is colored white.
Then resulting texture is rendered to the window framebuffer.
The size of the rendered texture and the window framebuffer may or may not be the same.
This gives us ability to send less rays for worse quality and better performance.
"Interpolation" can be used during this step.

### Data

`Data` folder contains icons, default saves, and shaders.

## Building

For building you will need a C++ compiler, make, cmake, OpenGL headers.

Run these commands:

``` bash
cmake -S . -B build
cmake --build build
```

Result will be in `dest` folder alongside with .zip file.

## Usage

### Controls

W - move forward \
S - move backward \
D - move right \
A - move left \
E - move up \
Q - move down

Shift - 5 times slower \
Control - 5 times faster

Alt+Z - open help window
Alt+X - open info window
Alt+C - open parameters window
Alt+V - open functions window

### Graphs

`blockFunction` and `displayFunction` are essentialy just GLSH code. For using the program right, follow rules that tell what constants, variables, and functions you are allowed to use.

Basics:
- GLSL syntax
- you can use any standard GLSL math function

### Block Function

Used to check if voxel is solid or not and get the type of voxel.

Function signature:
``` cxx
int blockFunction(args...) {
  @b // @b is replaced with your code
}
```

If `0` is returned ray calculation continues. Otherwise the `displayFunction` is called and calculation is ended. This value then is put into `displayFunction` as an argument.

Consts you can use:
- `float epsilon` - 1e-9
- `float pi` - to the tenth digit
- `float e` - to the tenth digit
- `float phi` - to the tenth digit

Vars you can use:
- `ivec3 coords` - voxel coords
- `ivec3 rayDirSign` - sign of every direction value, only 1 or -1
- `float t` - time from the start of the program in seconds

Funcs you can use:
- `void stop()` - stops calculating ray after this voxel
- `bool approx(float a, float b, float c)` - returns `true` if difference between `a` and `b` is smaller or equal to `c`

### Display Function

Used to determine the color of the pixel after the voxel is determined to be solid by `blockFunction`.

Function signature:
``` cxx
vec4 displayFunction(args...) {
  @d // @d is replaced with your code
}
```

The returned color will be applied to the pixel whose ray we just calculated.

Consts you can use:
- `float epsilon` - 1e-9
- `float pi` - to the tenth digit
- `float e` - to the tenth digit
- `float phi` - to the tenth digit

Vars you can use:
- `ivec3 coords` - voxel coords
- `float dist` - distance from the ray origin to the end
- `int lastCoord` - last axis that was crossed to get to this voxel. 0 - none, starting value. 1 - ox. 2 - oy. 3 - oz.
- `int blockValue` - return value of `blockFunction`
- `float t` - time from the start of the program in seconds

Funcs you can use:
- `bool approx(float a, float b, float c)` - returns `true` if difference between `a` and `b` is smaller or equal to `c`

## License

The license for this project is in `LICENSE` file. It is a modified MIT license.

The `extern` folder contains libraries that have their own licenses. This project's license is not applied to `extern` folder.

## Credits

### Voxel Grapher

Creator:
- [Stanislav Yatskiv | UAPROGRAMER](https://github.com/UAPROGRAMER)

### Used libraries and other

- [OpenGL](https://www.khronos.org/opengl)
- [GLAD](https://github.com/Dav1dde/glad)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [ImGui](https://github.com/ocornut/imgui)
- [mINI](https://github.com/metayeti/mINI)
- [stb](https://github.com/nothings/stb)

## Contact info

GitHub: [UAPROGRAMER](https://github.com/UAPROGRAMER) \
Email: stasyatskiu2008@gmail.com