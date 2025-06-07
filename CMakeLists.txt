cmake_minimum_required(VERSION 3.22)
project(3DASCII VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# SFML
FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.1
  GIT_SHALLOW ON
)
set(SFML_STATIC_LIBRARIES TRUE)
FetchContent_MakeAvailable(SFML)

# ImGui
FetchContent_Declare(ImGui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.1
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(ImGui)

# 👉 Set these BEFORE ImGui-SFML is declared
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR ${imgui_SOURCE_DIR})  # ✅ THIS is what fixes your error

# ImGui-SFML
FetchContent_Declare(ImGui-SFML
  GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
  GIT_TAG v3.0
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(ImGui-SFML)

# Your executable
add_executable(3DASCII
  src/main.cpp
)

target_include_directories(3DASCII PRIVATE ${imgui_SOURCE_DIR})
target_link_libraries(3DASCII PRIVATE
  ImGui-SFML::ImGui-SFML
  sfml-graphics
)
