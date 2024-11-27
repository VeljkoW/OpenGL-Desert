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
		void update(float deltaTime,float aspectRatio, bool isDay);
		void render();
		float getPosX() const;
		float getPosY() const;
		float getRadius() const;
		void setPosX(float x);
		void setPosY(float y);
	private:
		GLuint VAO, VBO;
		float speed;
		int numSegments;
		std::vector<GLfloat> vertices;
		float posX, posY, radius;

		GLuint shader;
		void createAndLoadShader();
		void createSunVertices();
};

#endif