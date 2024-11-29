#ifndef OASISSURFACE_H
#define OASISSURFACE_H

#include <vector>
#include <GL/glew.h>


class OasisSurface {
public:
	OasisSurface();
	~OasisSurface();
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
