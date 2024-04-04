#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

// Shaders
const char *vertexShaderSrc = "#version 330 core\n"
                              "layout (location = 0) in vec3 aPos;\n"
                              "void main()\n"
                              "{\n"
                              "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n"
                              "}\n\0";

const char *fragmentShaderSrc = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "void main()\n"
                                "{\n"
                                "  FragColor = vec4(0.8f, 0.3f, 0.92f, 1.0f);"
                                "}\n\0";

int main()
{

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window(glfwCreateWindow(500, 500, "Triangle", nullptr, nullptr));
    glfwMakeContextCurrent(window);

    // Graphics Pipeline:
    //
    //  input -> Vertex Data
    //
    //  1. Vertex Shader: Takes positions of all vertices and transforms them.
    //  2. Shape Assembly: Takes all positions and connects them according to a primitive (line, triangle...).
    //  3. Geometry Shader: Can add vertices and create new primitives out of existing primitives
    //  4. Rasterization: Transforms shapes into pixels
    //  5. Fragment Shader: Adds Color to the pixels
    //  6. Tests and Blending
    //

    gladLoadGL();

    // Load vertex Shader
    GLuint vertexShader(glCreateShader(GL_VERTEX_SHADER));
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);
    // Load fragment Shader
    GLuint fragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);
    // Create ShaderProgram and link our own shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Delete Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); // vertex array object -> stores multiple VBO's
    glBindVertexArray(VAO);     // Bind VAO
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    glGenBuffers(1, &VBO);              // vertex buffer object -> stores the vertex Data
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO to an arrayBuffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GLfloat vertices[] = {-0.5f, -0.5f, 0.0f,  // lower left
                          0.5f,  -0.5f, 0.0f,  // lower right
                          0.0f,  0.5f,  0.0f}; // uper

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Stores Vertices into a VBO
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Configure so that OpenGL know how to interpret the VBO's
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0); // enable it

    while (!glfwWindowShouldClose(window))
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}