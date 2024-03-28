# add executable and the corresponding headers
add_executable(${OPEN_GL_STARTER} main.cpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/include include)

# link glfw
find_package(OpenGL REQUIRED)
target_link_libraries(${OPEN_GL_STARTER} PRIVATE glfw)
target_link_libraries(${OPEN_GL_STARTER} PRIVATE OpenGL::GL)

# setup install target
install(TARGETS ${OPEN_GL_STARTER}
        FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
