#ifndef FISH_H
#define FISH_H

#include <vector>
#include <GL/glew.h>

class Fish {
public:
	Fish(float x, float y);
	~Fish();
	void render();
private:
	GLuint VAO, VBO;
	GLuint shader;
	float posX, posY;
	void createAndLoadShader();
	void setupVertices();
};


#endif
