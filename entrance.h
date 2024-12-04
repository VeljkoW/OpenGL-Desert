#ifndef ENTRANCE_H
#define ENTRANCE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Entrance {
public:
    Entrance(float baseWidth, float height, float xPos, float yPos);
    ~Entrance();

    void render();
    float getProgress() const;
    bool isAnimationStarted() const;
    void setProgress(float p);
    void setAnimationStarted(bool a);

private:
    GLuint VAO, VBO;
    GLuint shader;
    float progress;
    bool animationStarted;

    float vertices[8];
    void createAndLoadShader();

    float entranceLeftX, entranceLeftY;
    float entranceRightX, entranceRightY;
    float entranceLeftTopX, entranceLeftTopY;
    float entranceRightTopX, entranceRightTopY;
};

#endif // ENTRANCE_H