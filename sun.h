#ifndef SUN_H
#define SUN_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sun {
	public:
		Sun(float startPosX, float startPosY);
		void update(float deltaTime,float aspectRatio);
		void render();
		float posX, posY;
	private:
		GLuint VAO, VBO;
		float speed;
		int numSegments;
		float radius;
		std::vector<GLfloat> vertices;

		GLuint shader;
		void createAndLoadShader();
		void createSunVertices();
};

#endif