#include "grassBlades.h"
#include <iostream>
#include <sstream>

GrassBlades::GrassBlades() : alpha(1.0f)
{
    createAndLoadShader();

    // Define ellipse parameters (from oasis)
    const int numSegments = 50; // Number of grass blades
    const float centerX = -0.5f; // Center of the ellipse
    const float centerY = -0.5f;
    const float radiusX = 0.4f;  // Horizontal radius of the ellipse
    const float radiusY = 0.2f;  // Vertical radius of the ellipse
    const float grassHeight = 0.2f; // Height of each blade

    std::vector<float> vertices;

    // Loop through the bottom half of the ellipse (from π to 2π)
    for (int i = 0; i < numSegments; ++i) {
        // Calculate angle from π to 2π (for the entire bottom half of the ellipse)
        float angle = 3.1415926535897932384f + (i * (3.1415926535897932384f) / (numSegments - 1)); // From π to 2π

        // Calculate x and y for the ellipse position
        float x = centerX + radiusX * cos(angle);
        float y = centerY + radiusY * sin(angle);

        // Generate a blade of grass (a small triangle)
        vertices.push_back(x - 0.01f);   // Base-left of the blade
        vertices.push_back(y);
        vertices.push_back(0.0f);        // R
        vertices.push_back(0.8f);        // G (green grass)
        vertices.push_back(0.0f);        // B

        vertices.push_back(x);           // Tip of the blade
        vertices.push_back(y + grassHeight);
        vertices.push_back(0.0f);
        vertices.push_back(0.8f);
        vertices.push_back(0.0f);

        vertices.push_back(x + 0.01f);   // Base-right of the blade
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(0.8f);
        vertices.push_back(0.0f);
    }

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
GrassBlades::~GrassBlades()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}
void GrassBlades::render()
{
    glUseProgram(shader);

    // Set the alpha value
    glUniform1f(alphaLoc, alpha);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 150); // 50 blades * 3 vertices each
    glBindVertexArray(0);

    glUseProgram(0);
}
void GrassBlades::createAndLoadShader()
{
    const char* vertex = R"(
        #version 330 core
        layout(location = 0) in vec2 position; // Vertex position
        layout(location = 1) in vec3 color;    // Vertex color

        out vec3 fragColor; // Pass color to fragment shader

        void main() {
            fragColor = color;
            gl_Position = vec4(position, 0.0f, 1.0f);
        }
    )";

    const char* fragment = R"(
        #version 330 core
        in vec3 fragColor;  // Incoming color from vertex shader
        out vec4 color;     // Final fragment color

        uniform float alpha; // Alpha transparency

        void main() {
            color = vec4(fragColor, alpha);
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

    alphaLoc = glGetUniformLocation(shader, "alpha");
    if (alphaLoc == -1) {
        std::cerr << "Warning: Failed to find uniform 'alpha'!" << std::endl;
    }
}
void GrassBlades::setAlpha(float a)
{
    alpha = a;
}
float GrassBlades::getAlpha() const
{
    return alpha;
}