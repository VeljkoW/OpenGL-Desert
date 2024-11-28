#ifndef GRASSBLADES_H
#define GRASSBLADES_H

#include <vector>
#include <GL/glew.h>


class GrassBlades {
public:
	GrassBlades();
	~GrassBlades();
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
