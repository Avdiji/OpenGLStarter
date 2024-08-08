#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Shaders
const char *vertexShaderSrc =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n"
    "}\n\0";

const char *fragmentShaderSrc =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(0.8f, 0.3f, 0.92f, 1.0f);"
    "}\n\0";

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window(glfwCreateWindow(500, 500, "Triangle", nullptr, nullptr));
    glfwMakeContextCurrent(window);
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

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);  // vertex array object -> stores multiple VBO's
    glBindVertexArray(VAO);      // Bind VAO
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    glGenBuffers(1, &VBO);               // vertex buffer object -> stores the vertex Data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind VBO to an arrayBuffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    glGenBuffers(1, &EBO);                       // Element buffer Object -> determines in which order to draw triangles
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // Bind EBO to Element Array Buffer
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // This is the pyramid shape with all its vertices
    //
    //     5
    //    ---
    //   1   3
    //  --- ---
    // 0---2---4
    GLfloat vertices[] = {
        // Positions
        -0.5f,  -0.5f, 0.0f,  // 0
        -0.25f, 0.0f,  0.0f,  // 1
        0.0f,   -0.5f, 0.0f,  // 2
        0.25f,  0.0f,  0.0f,  // 3
        0.5f,   -0.5f, 0.0f,  // 4
        0.0f,   0.5f,  0.0f   // 5
    };

    GLuint indices[] = {
        0, 1, 2,  // lower left triangle
        2, 3, 4,  // lower right triangle
        1, 5, 3   // upper triangle
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Stores Vertices into a VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Configure so that OpenGL know how to interpret the VBO's
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);  // enable it

    // Unbind the VAO (it's always a good thing to unbind any buffer to prevent weird bugs)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);  // clear colors from previous frame

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete the buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}