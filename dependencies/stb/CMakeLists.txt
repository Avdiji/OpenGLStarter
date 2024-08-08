set(STB stb)
add_library(${STB} src/stb_image.cpp)
target_include_directories(${STB} PUBLIC include)