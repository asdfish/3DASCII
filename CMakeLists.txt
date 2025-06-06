cmake_minimum_required(VERSION 3.10)
project(3DASCII VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_executable(3DASCII
src/main.cpp
src/image.cpp
)

set_property(TARGET 3DASCII PROPERTY CXX_STANDARD 17)
target_include_directories(3DASCII PRIVATE include)