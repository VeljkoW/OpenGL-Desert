#include "oasisSurface.h"
#include <iostream>
#include <sstream>


OasisSurface::OasisSurface() : alpha (1.0f)
{

    const int numSegments = 100; 
    const float centerX = -0.5f;
    const float centerY = -0.5f;
    const float radiusX = 0.4f;
    const float radiusY = 0.2f;

    std::vector<float> vertices;
    for (int i = 0; i <= numSegments; ++i) {
        float angle = 2.0f * 3.1415926535897932384 * i / numSegments;
        float x = centerX + radiusX * cos(angle);
        float y = centerY + radiusY * sin(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.6f); 
        vertices.push_back(0.8f); 
        vertices.push_back(1.0f); 
        vertices.push_back(alpha); 
    }
    createAndLoadShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
OasisSurface::~OasisSurface()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}
void OasisSurface::render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shader);

    glUniform1f(alphaLoc, alpha);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 101); 
    glBindVertexArray(0);
    glUseProgram(0);
}
void OasisSurface::createAndLoadShader()
{
    const char* vertex = R"(
        #version 330 core
        layout(location = 0) in vec2 position; // Vertex position
        layout(location = 1) in vec3 color;    // Color attribute
        layout(location = 2) in float alpha;   // Alpha value for transparency

        out vec3 fragColor; // Output to fragment shader
        out float fragAlpha; // Output alpha value to fragment shader

        void main() {
            fragColor = color; // Pass color to the fragment shader
            fragAlpha = alpha; // Pass alpha to fragment shader
            gl_Position = vec4(position, 0.0f, 1.0f);  // Directly use the vertex position
        }
    )";

    const char* fragment = R"(
        #version 330 core
        in vec3 fragColor;  // Incoming color from vertex shader
        uniform float alpha; // Alpha value for transparency

        out vec4 color;     // Final color output

        void main() {
            color = vec4(fragColor, alpha);  // Output the color with alpha for transparency
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
        std::cerr << "Oasis surface vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Oasis surface fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Oasis surface shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    alphaLoc = glGetUniformLocation(shader, "alpha");
}
float OasisSurface::getAlpha() const {
    return alpha;
}
void OasisSurface::setAlpha(float a) {
    alpha = a;
}