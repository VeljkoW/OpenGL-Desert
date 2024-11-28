#ifndef OASIS_H
#define OASIS_H

#include <vector>
#include <GL/glew.h>


class Oasis {
public:
	Oasis();
	~Oasis();
	void render();

private:
	GLuint VAO, VBO;
	GLuint shader;
	void createAndLoadShader();
};

#endif
