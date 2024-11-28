#include "star.h"
#include <iostream>
#include <sstream>


Star::Star(float x, float y) : alpha(0.0f)
{
    GLfloat vertices[] = {
        x, y
    };
    createAndLoadShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
Star::~Star()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}

void Star::update(float brightness)
{
    float targetAlpha = 1.0f - brightness;

    float transitionSpeed = 0.2f;
    alpha += (targetAlpha - alpha) * transitionSpeed;

    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;
}

void Star::render()
{
    glUseProgram(shader);

    glUniform1f(alphaLoc, alpha);
    glPointSize(5.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1); // Render as a single point (star)
    glBindVertexArray(0);
}


void Star::createAndLoadShader() {
    const char* vertex = R"(
        #version 330 core
        layout(location = 0) in vec2 position; // Vertex position
        out vec2 fragPos; // Pass position to the fragment shader
        void main() {
            fragPos = position; // Pass position to fragment shader
            gl_Position = vec4(position, 0.0f, 1.0f); // Use vertex position
        }
    )";

    const char* fragment = R"(
         #version 330 core
        in vec2 fragPos; // Incoming position from vertex shader
        out vec4 color;  // Output color of the fragment
        uniform float alpha; // Transparency value for the star
        void main() {
            // Set the color to white with variable alpha
            color = vec4(1.0f, 1.0f, 1.0f, alpha); // White star with alpha transparency
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

    // **Retrieve the uniform location after the shader is linked**
    alphaLoc = glGetUniformLocation(shader, "alpha");
    if (alphaLoc == -1) {
        std::cerr << "Warning: Failed to find uniform 'alpha'!" << std::endl;
    }
}