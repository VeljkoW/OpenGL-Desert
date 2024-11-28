#include "oasis.h"
#include <iostream>
#include <sstream>


Oasis::Oasis()
{

    // Define the ellipse parameters
    const int numSegments = 100; // Higher for smoother ellipse
    const float centerX = -0.5f; // Center in the bottom-left quadrant
    const float centerY = -0.5f;
    const float radiusX = 0.4f;  // Horizontal radius
    const float radiusY = 0.2f;  // Vertical radius

    // Create vertices for the ellipse
    std::vector<float> vertices;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * 3.1415926535897932384 * i / numSegments;
        float x = centerX + radiusX * cos(angle);
        float y = centerY + radiusY * sin(angle);

        // Add position (x, y) and color (light blue)
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.6f); // R
        vertices.push_back(0.8f); // G
        vertices.push_back(1.0f); // B
    }
    createAndLoadShader();

    // Set up VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
Oasis::~Oasis()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}
void Oasis::render()
{
    glUseProgram(shader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 101); // 100 segments + center
    glBindVertexArray(0);
    glUseProgram(0);
}
void Oasis::createAndLoadShader()
{
    const char* vertex = R"(
        #version 330 core
        layout(location = 0) in vec2 position; // Vertex position
        layout(location = 1) in vec3 color;    // Color attribute

        out vec3 fragColor; // Output to fragment shader

        void main() {
            fragColor = color; // Pass color to the fragment shader
            gl_Position = vec4(position, 0.0f, 1.0f);  // Directly use the vertex position
        }
    )";

    const char* fragment = R"(
        #version 330 core
        in vec3 fragColor;  // Incoming color from vertex shader

        out vec4 color;     // Final color output

        void main() {
            color = vec4(fragColor, 1.0f);  // Output the color
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}