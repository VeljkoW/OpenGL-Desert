#ifndef FISH_H
#define FISH_H

#include <vector>
#include <GL/glew.h>

class Fish {
public:
	Fish(float x, float y);
	~Fish();
	void render();
	void updatePosition(float deltaTime);
private:
	GLuint VAO, VBO;
	GLuint shader;
	float posX, posY;
	bool movingRight;
	float speed;
	float direction;
	void createAndLoadShader();
	void setupVertices();
};


#endif
