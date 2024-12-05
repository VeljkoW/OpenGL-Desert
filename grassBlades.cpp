#include "grassBlades.h"
#include <iostream>
#include <sstream>

GrassBlades::GrassBlades() : alpha(1.0f)
{
    createAndLoadShader();

    const int numSegments = 50; 
    const float centerX = -0.5f; 
    const float centerY = -0.5f;
    const float radiusX = 0.4f;  
    const float radiusY = 0.2f;  
    const float grassHeight = 0.2f; 

    std::vector<float> vertices;

    for (int i = 0; i < numSegments; ++i) {
        float angle = 3.1415926535897932384f + (i * (3.1415926535897932384f) / (numSegments - 1)); 

        float x = centerX + radiusX * cos(angle);
        float y = centerY + radiusY * sin(angle);

        vertices.push_back(x - 0.01f);   
        vertices.push_back(y);
        vertices.push_back(0.0f);        
        vertices.push_back(0.8f);        
        vertices.push_back(0.0f);        

        vertices.push_back(x);           
        vertices.push_back(y + grassHeight);
        vertices.push_back(0.0f);
        vertices.push_back(0.8f);
        vertices.push_back(0.0f);

        vertices.push_back(x + 0.01f);  
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(0.8f);
        vertices.push_back(0.0f);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

    glUniform1f(alphaLoc, alpha);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 150); 
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
        std::cerr << "Grass blades vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Grass blades fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Grass blades shader Linking Error:\n" << infoLog << std::endl;
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