cmake_minimum_required(VERSION 3.10)
project(GLFW_Interface)

set(CMAKE_CXX_STANDARD 17)

# GLFW via FetchContent (cross-platform, no binary in repo)
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

add_executable(GLFW_Interface main.cpp GameOfLife.cpp Renderer.cpp Camera.cpp)

# Link GLFW and OpenGL (OpenGL32 for Windows, otherwise use OpenGL)
if (WIN32)
    target_link_libraries(GLFW_Interface glfw opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(GLFW_Interface glfw OpenGL::GL)
endif()

# Include directories for GLFW
# (FetchContent already handles include dirs)
