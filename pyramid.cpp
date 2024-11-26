#include "pyramid.h"
#include "stb_image.h"
#include <iostream>
#include <sstream>
#include <fstream>

Pyramid::Pyramid(const std::string& texturePath, float baseWidth, float height, float xPos, float yPos) {
    // Define vertices for two triangles to create a pyramid shape
    float vertices[] = {
        // First triangle (Left side of the pyramid)
        -baseWidth / 2.0f,  0.0f,   0.0f,    0.0f, 0.0f,  // Bottom-left
        0.0f,               -baseWidth / 4.0f, baseWidth / 4.0f,    1.0f, 0.0f,  // Bottom-right lowered
        0.0f,               height,   0.0f,    0.5f, 1.0f,  // Top (apex)

        // Second triangle (Right side of the pyramid)
        0.0f,               -baseWidth / 4.0f, baseWidth / 4.0f,    0.0f, 0.0f,  // Bottom-left lowered
        baseWidth / 2.0f,    0.0f,   0.0f,    1.0f, 0.0f,  // Bottom-right
        0.0f,               height,   0.0f,    0.5f, 1.0f   // Top (apex)
    };

    // Apply translation to vertices based on position offsets
    for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 5) {
        vertices[i] += xPos;  // Translate x
        vertices[i + 1] += yPos;  // Translate y
    }


    // Generate and bind the VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (u, v)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    createAndLoadShader();
    loadImageToTexture(texturePath);

}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &textureID);
    glDeleteProgram(shader);
}

void Pyramid::render() {
    glUseProgram(shader);
    if (textureID == 0) {
        std::cerr << "Error: Invalid texture ID" << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Draw both triangles

    // Deallocate
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Pyramid::loadImageToTexture(const std::string& texturePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(texturePath.c_str(), &TextureWidth, &TextureHeight, &TextureChannels, 0);

    if (ImageData != NULL) {
        std::cout << "Image loaded successfully from: " << texturePath.c_str()
            << " (Width: " << TextureWidth << ", Height: " << TextureHeight
            << ", Channels: " << TextureChannels << ")" << std::endl;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(ImageData);
    }
    else {
        std::cerr << "Failed to load image from: " << texturePath.c_str() << std::endl;
        stbi_image_free(ImageData);
        return;
    }
}

void Pyramid::createAndLoadShader() {
    const char* vertex = R"(
        #version 330 
        layout(location = 0) in vec3 inPos;
        layout(location = 1) in vec2 inTex;
        out vec2 chTex;
        void main()
        {
            gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
            chTex = inTex;
        }
    )";

    const char* fragment = R"(
        #version 330 core
        in vec2 chTex; 
        out vec4 outCol;

        uniform sampler2D uTex; 

        void main()
        {
            outCol = texture(uTex, chTex);
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
        std::cerr << "Pyramid vertex Shader Compilation Error:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Pyramid fragment Shader Compilation Error:\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Pyramid shader Linking Error:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
