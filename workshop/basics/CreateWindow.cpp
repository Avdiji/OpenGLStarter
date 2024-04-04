#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    // initialize opengl
    glfwInit();

    // giving hints, so that glfw knows which version we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // glfw version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // glfw version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL version 3.3, modern profile

    // Create Window (also check whether we were able to create it properly)
    GLFWwindow *window = glfwCreateWindow(500, 500, "CreateWindow", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Unable to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Add window to context

    gladLoadGL();               // use Glad to load opengl configurations
    glViewport(0, 0, 500, 500); // set viewport

    glClearColor(0.5f, 0.15f, 0.75f, 1.0f); // clear color of buffer and assign new color
    glClear(GL_COLOR_BUFFER_BIT);           // execute command on the color buffer
    glfwSwapBuffers(window);                // swap frontbuffer and backbuffer

    // Create while loop (as long as window is not closed)
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); // checks activities
    }

    // terminate everything
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}