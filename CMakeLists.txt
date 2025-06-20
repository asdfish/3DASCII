cmake_minimum_required(VERSION 3.22)
project(3DASCII VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# === SDL3 Setup ===
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3")
include_directories(${SDL3_DIR}/include)
link_directories(${SDL3_DIR}/lib/x64)

# === ImGui Setup ===
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/lib/imgui")

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp  # Optional
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

# === Source Files ===
set(SRC_FILES
    # Core
    src/core/main.cpp
    # # Managers
    src/core/managers/Coordinator.cpp
    src/core/managers/EventBus.cpp 
    src/core/managers/EntityManager.cpp 
    src/core/managers/ComponentManager.cpp 
    src/core/managers/SystemManager.cpp
)

# === Executable ===
add_executable(3DASCII
    ${SRC_FILES}
    ${IMGUI_SRC}
)

# === Includes ===
target_include_directories(3DASCII PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL3_DIR}/include
    ${PROJECT_SOURCE_DIR}/include-test
)

# === Link Libraries ===
find_package(OpenGL REQUIRED)

target_link_libraries(3DASCII PRIVATE
    SDL3           # From link_directories
    OpenGL::GL     # From find_package
)

# Optional: Copy SDL3.dll to output (for runtime on Windows)
if (WIN32)
    add_custom_command(TARGET 3DASCII POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_DIR}/lib/x64/SDL3.dll"
        $<TARGET_FILE_DIR:3DASCII>)
endif()