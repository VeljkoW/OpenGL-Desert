#include "fish.h"
#include <iostream>
#include <sstream>
#include <GLFW/glfw3.h>

Fish::Fish(float x, float y) : posX(x), posY(y), movingRight(true), speed(0.01f), direction(-1.0f)
{
    setupVertices();
    createAndLoadShader();
}
Fish::~Fish()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}
void Fish::updatePosition(float deltaTime)
{
    if (movingRight) {
        posX += speed * deltaTime;
        if (posX >= -0.3f) {  
            movingRight = false;  
            direction = 1.0f;   
        }
    }
    else {
        posX -= speed * deltaTime;
        if (posX <= -0.7f) { 
            movingRight = true;  
            direction = -1.0f;     
        }
    }
}
void Fish::render()
{
    glUseProgram(shader);

    GLint posLocation = glGetUniformLocation(shader, "fishPos");
    glUniform2f(posLocation, posX, posY);

    GLint flipLocation = glGetUniformLocation(shader, "flipDirection");
    glUniform1f(flipLocation, direction); 

    glBindVertexArray(VAO);
    //for the body
    glDrawArrays(GL_TRIANGLE_FAN, 0, 102);
    //for the tail
    glDrawArrays(GL_TRIANGLES, 102, 3);
    //for the eye
    glDrawArrays(GL_TRIANGLE_FAN, 105, 102); 

    glBindVertexArray(0);
}
void Fish::setupVertices()
{
    std::vector<float> vertices;

    const int segments = 100;
    const float radiusX = 0.05f;
    const float radiusY = 0.03333f; 

    vertices.push_back(0.0f);  
    vertices.push_back(0.0f);  
    vertices.push_back(1.0f);  
    vertices.push_back(0.5f);  
    vertices.push_back(0.0f);  

    for (int i = 0; i <= segments; ++i) {
        float angle = (2.0f * 3.1456 * i) / segments;
        float x = radiusX * cos(angle); 
        float y = radiusY * sin(angle); 

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(1.0f);  
        vertices.push_back(0.5f);  
        vertices.push_back(0.0f);  
    }

    float triangleVertices[] = {
        0.01666666f,  0.0f,  1.0f, 0.5f, 0.0f,  
        0.08333333f, -0.04f, 1.0f, 0.5f, 0.0f,  
        0.08333333f,  0.04f, 1.0f, 0.5f, 0.0f,  
    };

    vertices.insert(vertices.end(), std::begin(triangleVertices), std::end(triangleVertices));

    const float eyeCenterX = -0.03f;
    const float eyeCenterY = 0.01f; 
    const float eyeRadius = 0.005f;

    vertices.push_back(eyeCenterX);
    vertices.push_back(eyeCenterY);
    vertices.push_back(0.0f);  
    vertices.push_back(0.0f);  
    vertices.push_back(0.0f); 

    for (int i = 0; i <= segments; ++i) {
        float angle = (2.0f * 3.1456 * i) / segments;
        float x = eyeCenterX + eyeRadius * cos(angle);
        float y = eyeCenterY + eyeRadius * sin(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);  
        vertices.push_back(0.0f);  
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
void Fish::createAndLoadShader()
{
    const char* vertex = R"(
        #version 330 core
        layout(location = 0) in vec2 position;
        layout(location = 1) in vec3 color;

        uniform vec2 fishPos; // Position offset
        uniform float flipDirection; // Flip the fish

        out vec3 fragColor;

        void main() {
            fragColor = color;
            gl_Position = vec4(position.x * flipDirection + fishPos.x, position.y + fishPos.y, 0.0f, 1.0f);
        }
    )";

    const char* fragment = R"(
       #version 330 core
        in vec3 fragColor;
        out vec4 color;

        void main() {
            color = vec4(fragColor, 1.0f);
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