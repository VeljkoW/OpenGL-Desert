#include "Sun.h"
#include <iostream>

Sun::Sun(float startPosX, float startPosY) : posX(startPosX), posY(startPosY), speed(0.1f) 
{
    numSegments = 360;
    radius = 0.1f;
    createSunVertices();
    createAndLoadShader();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO and upload the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Position attribute (x, y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VAO
    glBindVertexArray(0);
}
Sun::~Sun()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}

void Sun::update(float deltaTime, float aspectRatio,bool isDay)
{
    posX -= speed * deltaTime; // Move the sun to the left

    // Reset position when it goes off-screen
    if (posX < -1.0f - radius && !isDay) {
        posX = 1.0f + radius; // Re-enter from the right side
    }

    // Parameters for the sine wave
    float waveAmplitude = 0.15f;  // Max height of the curve
    float waveWidth = 4.0f;       // Controls how wide the curve is
    float centerScreen = 0.6f;    // Baseline for the curve (height of the sun)

    // Normalize posX to [0, 2π] for a full sine wave period
    float normalizedX = (posX + 1.0f) * 3.14159f / 2.0f; // Mapping [-1, 1] to [0, 2π]

    // Calculate the sine curve position
    posY = centerScreen + waveAmplitude * sin(normalizedX);

    // Update the sun's position in the vertex buffer (for rendering)
    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * 2.0f * 3.14159f / numSegments;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        vertices[i * 5] = x / aspectRatio + posX; // Adjust x for aspect ratio
        vertices[i * 5 + 1] = y + posY;          // y remains unchanged
    }

    // Update VBO with new position data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
}



void Sun::render() {
    glUseProgram(shader);

    // Bind the VAO and draw the sun
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 1); // Draw the circle (fan of triangles)
    glBindVertexArray(0);
}

void Sun::createSunVertices()
{
    for (int i = 0; i <= numSegments; ++i) {
        float angle = (i * 2.0f * 3.14159f) / numSegments;
        vertices.push_back(cos(angle) * radius); // x
        vertices.push_back(sin(angle) * radius); // y
        vertices.push_back(1.0f);  // Color: Yellow (R)
        vertices.push_back(1.0f);  // Color: Yellow (G)
        vertices.push_back(0.0f);  // Color: Yellow (B)
    }
}

void Sun::createAndLoadShader()
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
        std::cerr << "Sand vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Sand fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Sand shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
float Sun::getPosX() const {
    return posX;
}
float Sun::getPosY() const {
    return posY;
}
float Sun::getRadius() const {
    return radius;
}
void Sun::setPosX(float x) {
    posX = x;
}
void Sun::setPosY(float y) {
    posY = y;
}