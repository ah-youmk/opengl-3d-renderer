cmake_minimum_required(VERSION 3.10)
project(opengl-3d-renderer)

set(CMAKE_CXX_STANDARD 17)

# Path to GLFW built with MinGW
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/external/glfw-3.4")
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIBRARY "${GLFW_ROOT}/build/src/libglfw3.a")

# Add source files
set(SOURCES
    src/main.cpp
    src/gl.c
)

add_executable(opengl-3d-renderer ${SOURCES})

# Include directories
target_include_directories(opengl-3d-renderer PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(opengl-3d-renderer PRIVATE ${GLFW_LIBRARY})
target_link_libraries(opengl-3d-renderer PRIVATE opengl32)