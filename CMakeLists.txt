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
    src/entity.cpp
    src/hitbox.cpp
)

set(PROJECT_HEADER_FILES
    src/block.h
    src/chunk.h
    src/dimension.h
    src/player.h
    src/py.h
    src/states.h
    src/world.h
    src/entity.h
    src/hitbox.h
)

set(PROJECT_PYTHON_FILES
    data/py/init.py
    data/textures/blocks/stone.png
)

set(PY_OUT ".")

foreach(file IN LISTS PROJECT_PYTHON_FILES)
    message(STATUS "${CMAKE_BINARY_DIR}/$<CONFIG>/${file}")
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/$<CONFIG>/${file}"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_BINARY_DIR}/$<CONFIG>/${file}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${file}"
    )
    list(APPEND PY_OUT "${CMAKE_BINARY_DIR}/$<CONFIG>/${file}")
endforeach()

list(POP_FRONT PY_OUT)

add_custom_target(PyData ALL DEPENDS ${PY_OUT})

include_directories("${CMAKE_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src/lib/raylib/src")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/lib/raylib")
add_executable(kod-craft)
target_compile_features(kod-craft PRIVATE cxx_std_20)
add_compile_definitions(_USE_MATH_DEFINES)
add_dependencies(kod-craft PyData)
target_sources(kod-craft PRIVATE ${PROJECT_SOURCE_FILES} 
    PRIVATE FILE_SET HEADERS BASE_DIRS ${CMAKE_SOURCE_DIR} FILES ${PROJECT_HEADER_FILES}
)
target_link_libraries(kod-craft "raylib" Python::Python)
install(TARGETS kod-craft)