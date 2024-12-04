#include "Entrance.h"
#include <iostream>

Entrance::Entrance(float baseWidth, float height, float xPos, float yPos)
    : progress(0.0f), animationStarted(false) {
    // Define initial vertices (static bottom, dynamic top based on progress)
    float entranceWidth = baseWidth / 4.0f;  // Entrance width
    float entranceHeight = height / 2.0f;    // Entrance height

    // Bottom-left and bottom-right are on the slanted base of the left triangle
    float leftBaseX = -baseWidth / 2.0f + xPos; // Leftmost base vertex of the triangle
    float rightBaseX = 0.0f + xPos;            // Shared base vertex of the triangle
    float baseY = yPos;                        // Shared Y-coordinate for the base

    // Compute downward slope for the slanted base
    float baseSlope = (-baseWidth / 4.0f - 0.0f) / (rightBaseX - leftBaseX); // Downward slope of the left triangle

    // Slightly lower the entrance to match the pyramid's bottom alignment
    float offset = baseWidth / 4.0f * 0.25f;  // A small offset, 10% of the pyramid's base width

    entranceLeftX = leftBaseX + entranceWidth / 2.0f;
    entranceLeftY = baseY - offset;          
    
    entranceRightX = rightBaseX - entranceWidth / 2.0f;
    entranceRightY = baseY + baseSlope * entranceWidth - offset; 

    entranceLeftTopX = leftBaseX + entranceWidth / 2.0f + 0.06f;
    entranceLeftTopY = entranceLeftY + entranceHeight - 0.03;

    entranceRightTopX = rightBaseX - entranceWidth / 2.0f;
    entranceRightTopY = entranceRightY + entranceHeight;

    // Define vertices for the entrance (two triangles forming a quadrilateral)
    vertices[0] = entranceLeftX;    
    vertices[1] = entranceLeftY;                      

    vertices[2] = entranceRightX;  
    vertices[3] = entranceRightY;                   

    vertices[4] = entranceLeftX;    // INITIALLY SET TO BE THE SAME AS THE BOTTOM ONES
    vertices[5] = entranceLeftY;

    vertices[6] = entranceRightX;
    vertices[7] = entranceRightY;

    // Set up VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // Now using 2 components (X, Y)
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    createAndLoadShader();
}

Entrance::~Entrance() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
}


void Entrance::render() {
    glUseProgram(shader);

    if (animationStarted && progress < 1.0f) {
        progress += 0.01f; 
        if (progress > 1.0f) {
            progress = 1.0f;
        }

        vertices[4] = entranceLeftX + progress * (entranceLeftTopX - entranceLeftX);
        vertices[5] = entranceLeftY + progress * (entranceLeftTopY - entranceLeftY);  

        vertices[6] = entranceRightX + progress * (entranceRightTopX - entranceRightX); 
        vertices[7] = entranceRightY + progress * (entranceRightTopY - entranceRightY); 

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(0);
}

void Entrance::createAndLoadShader() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        void main() {
           gl_Position = vec4(position, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 fragColor;
    
        void main() {
            fragColor = vec4(0.0, 0.0, 0.0, 1.0); // Adjust alpha based on progress
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
float Entrance::getProgress() const {
    return progress;
}
bool Entrance::isAnimationStarted() const {
    return animationStarted;
}
void Entrance::setProgress(float p)
{
    progress = p;
}
void Entrance::setAnimationStarted(bool a)
{
    animationStarted = a;
}