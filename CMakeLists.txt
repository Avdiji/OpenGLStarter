cmake_minimum_required(VERSION 3.28)
project(OpenGlStarter)

set(CMAKE_CXX_STANDARD 17)
set(OPEN_GL_STARTER OpenGLStarter)

# Setup OpenGL
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies)

add_subdirectory(src)

