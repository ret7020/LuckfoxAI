cmake_minimum_required(VERSION 3.5)

project(OpenCV_CSI_Camera)
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_COMPILER "$ENV{GCC_COMPILER}-gcc")
SET(CMAKE_CXX_COMPILER "$ENV{GCC_COMPILER}-g++")
SET(CMAKE_C_LINK_EXECUTABLE "$ENV{GCC_COMPILER}-ld")
set(CMAKE_SYSTEM_PROCESSOR arm)

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/opencv-mobile-4.10.0-luckfox-pico/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(OpenCV_CSI_Camera main.cpp)

target_link_libraries(OpenCV_CSI_Camera ${OpenCV_LIBS})
