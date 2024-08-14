#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

// Function prototypes
GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Window dimensions
const int width = 800;
const int height = 800;

// Vertex Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec2 u_resolution;
    uniform vec2 u_center;
    uniform float u_scale;
    uniform int u_maxIterations;

    void main() {
        vec2 c = u_center + (gl_FragCoord.xy - u_resolution / 2.0) * u_scale;
        vec2 z = vec2(0.0);
        int i;

        for (i = 0; i < u_maxIterations; i++) {
            if (length(z) > 2.0) break;
            z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        }

        float color = float(i) / float(u_maxIterations);
        FragColor = vec4(vec3(color), 1.0);
    }
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, "Mandelbrot Set", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set the viewport
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile shaders and create a shader program
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Vertex data for a full-screen quad
    float vertices[] = {
        -1.0f, -1.0f,  // Bottom-left
         1.0f, -1.0f,  // Bottom-right
        -1.0f,  1.0f,  // Top-left
         1.0f,  1.0f   // Top-right
    };

    // Index data for two triangles forming the quad
    unsigned int indices[] = {
        0, 1, 2,  // First triangle
        2, 1, 3   // Second triangle
    };

    // Setup VAO, VBO, and EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float scale = 3.5f / width;
    std::pair<float, float> center = {-0.5f, 0.0f};

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Zoom in (W key)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            scale *= 0.9f;  // Zoom in
        }

        // Zoom out (S key)
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            scale /= 0.9f;  // Zoom out
        }

        // Move up (UP arrow key)
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            center.second += scale * 0.1f;
        }

        // Move down (DOWN arrow key)
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            center.second -= scale * 0.1f;
        }

        // Move left (LEFT arrow key)
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            center.first -= scale * 0.1f;
        }

        // Move right (RIGHT arrow key)
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            center.first += scale * 0.1f;
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Set uniforms
        glUniform2f(glGetUniformLocation(shaderProgram, "u_resolution"), width, height);
        glUniform2f(glGetUniformLocation(shaderProgram, "u_center"), center.first, center.second);
        glUniform1f(glGetUniformLocation(shaderProgram, "u_scale"), scale);
        glUniform1i(glGetUniformLocation(shaderProgram, "u_maxIterations"), 10000);

        // Draw the full-screen quad using the index buffer
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// Function to compile a shader
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error compiling shader:\n" << infoLog << '\n';
    }

    return shader;
}

// Function to create a shader program
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Error linking program:\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Callback function to adjust the viewport size when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
