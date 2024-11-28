#ifndef GRASSBED_H
#define GRASSBED_H

#include <vector>
#include <GL/glew.h>

class GrassBed {
public:
	GrassBed();
	~GrassBed();
	void render();
	float getAlpha() const;
	void setAlpha(float a);

private:
	GLuint VAO, VBO;
	GLuint shader;
	GLint alphaLoc;
	float alpha;
	void createAndLoadShader();
};


#endif
