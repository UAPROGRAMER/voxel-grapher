cmake_minimum_required(VERSION 3.16)
project(voxel-grapher-project LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_CURRENT_SOURCE_DIR}/dest/Voxel Grapher"
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# GLAD

file(GLOB glad CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/src/glad.c
)

# GLFW

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(extern/glfw-3.4)

# IMGUI

file(GLOB imgui CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_glfw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui/backends/imgui_impl_opengl3.cpp
)

# SOURCE

file(GLOB source CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# EXECUTABLE

find_package(OpenGL REQUIRED)

if(WIN32)
  add_executable(voxel-grapher WIN32 ${glad} ${source}
    ${imgui} voxel-grapher.rc)
  target_link_libraries(voxel-grapher glfw OpenGL::GL)
  target_link_options(voxel-grapher PRIVATE "-static")
elseif(UNIX AND NOT APPLE)
  add_executable(voxel-grapher ${glad} ${source} ${imgui})
  target_link_libraries(voxel-grapher glfw OpenGL::GL dl pthread)
  target_link_options(
    voxel-grapher PRIVATE "-static-libgcc" "-static-libstdc++"
  )
endif()

target_include_directories(voxel-grapher PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/glad/include
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw-3.4/include
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/glm/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/mINI/src
  ${CMAKE_CURRENT_SOURCE_DIR}/extern/
)

set_target_properties(voxel-grapher PROPERTIES
  OUTPUT_NAME "Voxel Grapher"
)

# WARNINGS

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wfloat-equal
  -Wformat=2
  -Wformat-security
  -Wnull-dereference
  -Wshadow
  -Wpointer-arith
  -Wcast-align
  -Wmissing-declarations
  -Woverloaded-virtual
  -Wnon-virtual-dtor
  -Wunused
  -Wuninitialized
)

# CUSTOM COMMANDS

add_custom_command(TARGET voxel-grapher POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:voxel-grapher>/data
)

add_custom_target(zip ALL
  COMMAND ${CMAKE_COMMAND} -E tar cfv
    "${CMAKE_CURRENT_SOURCE_DIR}/dest/Voxel Grapher.zip"
    --format=zip
    "Voxel Grapher"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dest
  DEPENDS voxel-grapher
)