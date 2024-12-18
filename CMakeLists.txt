cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(kod-craft CXX)

find_package(Python REQUIRED Development)

set(PROJECT_SOURCE_FILES 
    src/main.cpp
    src/player.cpp
    src/world.cpp
    src/chunk.cpp
    src/block.cpp
    src/dimension.cpp
    src/py.cpp
)

set(PROJECT_HEADER_FILES
    src/block.h
    src/chunk.h
    src/dimension.h
    src/player.h
    src/py.h
    src/states.h
    src/world.h
)

include_directories("${CMAKE_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src/lib/raylib/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/lib/raylib")
add_executable(kod-craft)
target_sources(kod-craft PRIVATE ${PROJECT_SOURCE_FILES} 
    PRIVATE FILE_SET HEADERS BASE_DIRS ${CMAKE_SOURCE_DIR} FILES ${PROJECT_HEADER_FILES}
)
target_link_libraries(kod-craft "raylib" Python::Python)
install(TARGETS kod-craft)