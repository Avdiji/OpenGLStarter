find_package(glfw3 REQUIRED)

add_executable(Shaders Shaders.cpp)
target_link_libraries(Shaders glfw)
target_link_libraries(Shaders Glad)

add_executable(Mandelbrot Mandelbrot.cpp)
target_link_libraries(Mandelbrot glfw)
target_link_libraries(Mandelbrot Glad)

add_executable(Textures Textures.cpp)
target_link_libraries(Textures glfw)
target_link_libraries(Textures Glad)
target_link_libraries(Textures stb)
