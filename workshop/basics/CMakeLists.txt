add_executable(Create_Window CreateWindow.cpp)
target_link_libraries(Create_Window glfw)
target_link_libraries(Create_Window Glad)

add_executable(Triangle Triangle.cpp)
target_link_libraries(Triangle glfw)
target_link_libraries(Triangle Glad)