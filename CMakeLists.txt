cmake_minimum_required(VERSION 3.10)
project(opengl-3d-renderer)

set(CMAKE_CXX_STANDARD 17)

# Path to GLFW built with MinGW
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/external/glfw-3.4")
set(GLFW_INCLUDE_DIR "${GLFW_ROOT}/include")
set(GLFW_LIBRARY "${GLFW_ROOT}/build/src/libglfw3.a")
set(ASSIMP_DIR "C:/Program Files (x86)/Assimp/lib/cmake/assimp-6.0")

link_directories("C:/Program Files (x86)/assimp/lib")
find_package(assimp REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/glad.c
    src/stb_image.cpp
)

add_executable(opengl-3d-renderer ${SOURCES})

# Include directories
target_include_directories(opengl-3d-renderer PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${GLFW_INCLUDE_DIR}
)

target_include_directories(opengl-3d-renderer PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/external/glm
)

target_include_directories(opengl-3d-renderer PRIVATE ${CMAKE_SOURCE_DIR}/external/glm/glm)

# Link libraries
target_link_libraries(opengl-3d-renderer PRIVATE ${GLFW_LIBRARY})
target_link_libraries(opengl-3d-renderer PRIVATE opengl32)
target_link_libraries(opengl-3d-renderer PRIVATE assimp)