#ifndef SAND_H
#define SAND_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Sand {
	public:
		Sand(const std::string& texturePath);
		~Sand();
		void render();

	private:
		GLuint VAO, VBO;
		GLuint textureID;
		GLuint shader;
		void loadImageToTexture(const std::string& texturePath);
		void createAndLoadShader();
};


#endif