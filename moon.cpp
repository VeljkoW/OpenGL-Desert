#include "Moon.h"
#include <cmath>
#include <iostream>
#include <sstream>

Moon::Moon(float startPosX, float startPosY)
    : posX(startPosX), posY(startPosY), speed(0.1f) {
    numSegments = 360;
    radius = 0.1f;
    createMoonVertices();
    createAndLoadShader();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
Moon::~Moon()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}

void Moon::update(float deltaTime, float aspectRatio, bool isDay) {
    posX -= speed * deltaTime; 

    if (posX < -1.0f - radius && isDay) {
        posX = 1.0f + radius;
    }

    float waveAmplitude = 0.3f;
    float waveWidth = 4.0f;
    float centerScreen = 0.6f;
    float normalizedX = (posX + 1.0f) * 3.14159f / 2.0f;

    posY = centerScreen + waveAmplitude * sin(normalizedX) - 0.1f;

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * 2.0f * 3.14159f / numSegments;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        vertices[i * 5] = x / aspectRatio + posX;
        vertices[i * 5 + 1] = y + posY;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
}

void Moon::render() {
    glUseProgram(shader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1);
    glBindVertexArray(0);
}

void Moon::createMoonVertices() {
    for (int i = 0; i <= numSegments; ++i) {
        float angle = (i * 2.0f * 3.14159f) / numSegments;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        float red, green, blue;
        if (angle >= 3.4159f) {  
            red = green = blue = 0.0f;
        }
        else {  
            red = green = blue = 0.7f;
        }

        vertices.push_back(x);     
        vertices.push_back(y);     
        vertices.push_back(red);   
        vertices.push_back(green); 
        vertices.push_back(blue);  
    }
}

void Moon::createAndLoadShader() {
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
            // Directly use the color passed from the vertex shader
            color = vec4(fragColor, 1.0f);  // Combine color with alpha
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
        std::cerr << "Moon vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Moon fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Moon shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
float Moon::getPosX() const {
    return posX;
}
float Moon::getPosY() const {
    return posY;
}
float Moon::getRadius() const {
    return radius;
}
float Moon::getSpeed() const {
    return speed;
}
void Moon::setPosX(float x) {
    posX = x;
}
void Moon::setPosY(float y) {
    posY = y;
}
void Moon::setSpeed(float s) {
    speed = s;
}