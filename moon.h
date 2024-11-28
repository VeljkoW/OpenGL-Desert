#ifndef MOON_H
#define MOON_H

#include <vector>
#include <GL/glew.h>

class Moon {
    public:
        Moon(float startPosX, float startPosY);
        ~Moon();
        void update(float deltaTime, float aspectRatio,bool isDay);
        void render();
        float getPosX() const;
        float getPosY() const;
        float getRadius() const;
        float getSpeed() const;
        void setPosX(float x);
        void setPosY(float y);
        void setSpeed(float s);
    private:
        void createMoonVertices();
        void createAndLoadShader();
        float posX, posY, speed, radius;

        GLuint VAO, VBO;
        GLuint shader;
        std::vector<float> vertices;
        int numSegments;
};

#endif
