#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "sand.h"
#include "pyramid.h"
#include "sun.h"

float calculateDeltaTime();

int main(void)
{
    
    if (!glfwInit()) 
    {
        std::cout<<"GLFW Library not loaded! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    unsigned int wWidth = mode->width;
    unsigned int wHeight = mode->height;

    const char wTitle[] = "Desert Landscape";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, glfwGetPrimaryMonitor(), NULL);
    if (window == NULL) 
    {
        std::cerr << "Failed to create GLFW window. Terminating." << std::endl;
        glfwTerminate();
        return 2;
    }
    else {
        std::cout << "GLFW window created successfully." << std::endl;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        return 3;
    }
    else {
        std::cout << "GLEW initialized successfully." << std::endl;
    }

    glViewport(0, 0, wWidth, wHeight);

    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    }
    else {
        std::cerr << "Failed to retrieve OpenGL version" << std::endl;
    }


    Sand sand("sand.jpg");

    Pyramid pyramid1("brick.jpg", 0.35f, 0.4f, 0.5f, -0.1f);
    Pyramid pyramid2("brick.jpg", 0.30f, 0.32f, 0.8f, -0.3f);  
    Pyramid pyramid3("brick.jpg", 0.27f, 0.3f, 0.46f, -0.5f); 

    Sun sun(0.0f, 0.6f);

    float aspectRatio = static_cast<float>(wWidth) / static_cast<float>(wHeight);

    while (!glfwWindowShouldClose(window)) 
    {
        float deltaTime = calculateDeltaTime();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glClearColor(0.0f, 0.6f, 1.0f, 1.0f);

        sun.update(deltaTime,aspectRatio);

        glClear(GL_COLOR_BUFFER_BIT);

        sun.render();

        sand.render();

        pyramid1.render();
        pyramid2.render();
        pyramid3.render();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

float calculateDeltaTime() {
    static float lastFrameTime = 0.0f; // Time of the last frame
    float currentFrameTime = glfwGetTime(); // Get the current time
    float deltaTime = currentFrameTime - lastFrameTime; // Calculate the elapsed time
    lastFrameTime = currentFrameTime; // Update the last frame time
    return deltaTime; // Return the time difference
}