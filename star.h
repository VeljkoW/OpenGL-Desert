#ifndef STAR_H
#define STAR_H

#include <GL/glew.h>
#include <glm/glm.hpp>


class Star {
	public:
		Star(float x, float y);
		~Star();
		void render();
		void update(float skyBrightness);

	private:
		float alpha;
		GLuint VAO, VBO;
		GLuint shader;
		void createAndLoadShader();
		GLint alphaLoc;


};
#endif
