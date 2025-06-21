cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(3DASCII VERSION 0.1.0 LANGUAGES C CXX)

# === Project Source Files ===
set(SRC_FILES
    src/core/main.cpp
    src/core/managers/Coordinator.cpp
    src/core/managers/EventBus.cpp 
    src/core/managers/EntityManager.cpp 
    src/core/managers/ComponentManager.cpp 
    src/core/managers/SystemManager.cpp
)

# === Executable Target ===
add_executable(3DASCII ${SRC_FILES})

# === Include Directories ===
target_include_directories(3DASCII PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# === GLAD ===
add_library(glad STATIC
    lib/glad/src/gl.c
)
target_include_directories(glad PUBLIC lib/glad/include)

# === GLM ===
add_subdirectory(lib/glm)

# === GLFW ===
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

# === ImGui ===
file(GLOB IMGUI_SRC
    lib/imgui/*.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    lib/imgui
    lib/imgui/backends
    lib/glfw/include
)

# === Link All Dependencies ===
find_package(OpenGL REQUIRED)

target_link_libraries(3DASCII PRIVATE
    glad
    glm
    glfw
    imgui
    OpenGL::GL
)

# === Add ImGui + GLAD include dirs to your app ===
target_include_directories(3DASCII PRIVATE
    lib/glad/include
    lib/glm
    lib/imgui
    lib/imgui/backends
    lib/glfw/include
)



