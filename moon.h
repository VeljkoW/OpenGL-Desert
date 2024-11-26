#ifndef MOON_H
#define MOON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Moon {
	public:
		Moon(float startPosX, float startPosY);
		void update(float deltaTime);
		void render();
	private:
		float posX, posY;
		float speed;
};

#endif 
