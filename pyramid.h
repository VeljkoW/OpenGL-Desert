#ifndef PYRAMID_H
#define PYRAMID_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Pyramid {
public:
	Pyramid(const std::string& texturePath, float baseWidth, float height, float xPos, float yPos);
	~Pyramid();
	void render();

private:
	GLuint VAO, VBO;
	GLuint textureID;
	GLuint shader;
	void loadImageToTexture(const std::string& texturePath);
	void createAndLoadShader();
};


#endif