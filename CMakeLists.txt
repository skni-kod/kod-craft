cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(kod-craft CXX)

find_package(Python REQUIRED Development)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src/lib/raylib/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/lib/raylib")
add_executable(kod-craft
    src/main.cpp
    src/player.cpp
    src/world.cpp
    src/chunk.cpp
    src/block.cpp
    src/dimension.cpp
    src/py.cpp
)
target_link_libraries(kod-craft "raylib" Python::Python)
