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
#include "moon.h"
#include "star.h"


std::vector<std::unique_ptr<Star>> stars;

float calculateDeltaTime();
void createStars(int numberOfStars);
void updateStars(float deltaTime, float skyR, float skyG, float skyB);
void renderStars();

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

    bool isDay = true;
    float skyR = 0.0f, skyG = 0.6f, skyB = 1.0f;    //The color of the sky in the beggining

    Sun sun(0.0f, 0.6f);
    Moon moon(1.0f + 0.1f, 0.6f);

    float aspectRatio = static_cast<float>(wWidth) / static_cast<float>(wHeight);

    createStars(15);

    while (!glfwWindowShouldClose(window)) 
    {
        float deltaTime = calculateDeltaTime();
        if (isDay) {
            sun.update(deltaTime, aspectRatio,isDay);

            if (sun.getPosX() > -0.75f - sun.getRadius())
            {
                skyR = std::max(0.0f, skyR - 0.001f);
                skyG = std::min(0.6f, skyG + 0.001f);
                skyB = std::min(1.0f, skyB + 0.001f);
            }
            else if (sun.getPosX() > -0.97f - sun.getRadius())
            {
                skyR = std::min(247.0f / 255.0f, skyR + 0.004f);  
                skyG = std::min(147.0f / 255.0f, skyG + 0.003f);  
                skyB = std::max(114.0f / 255.0f, skyB - 0.003f);
            }
            else
            {
                skyR = std::max(0.0f, skyR - 0.004f);
                skyG = std::max(0.0f, skyG - 0.003f);
                skyB = std::max(0.0f, skyB - 0.003f);
            }
            if (sun.getPosX() < -1.0f - sun.getRadius())
            {
                isDay = false;
                moon.setPosX(1.0f + moon.getRadius());
                moon.update(deltaTime, aspectRatio, isDay);
            }
        }
        else {
            moon.update(deltaTime, aspectRatio, isDay);

            if (moon.getPosX() > -0.88 - moon.getRadius())
            {
                skyR = std::max(0.0f, skyR - 0.004f);
                skyG = std::max(0.0f, skyG - 0.003f);
                skyB = std::max(0.0f, skyB - 0.003f);
            }
            else
            {
                skyR = std::max(0.0f, skyR - 0.001f);
                skyG = std::min(0.6f, skyG + 0.001f);
                skyB = std::min(1.0f, skyB + 0.001f);
            }

            if (moon.getPosX() < -1.0f - moon.getRadius()) {
                isDay = true;
                sun.setPosX(1.0f + sun.getRadius());
            }
        }

        if (skyR < 0.0f) skyR = 0.0f;
        if (skyR > 1.0f) skyR = 1.0f;
        if (skyG < 0.0f) skyG = 0.0f;
        if (skyG > 1.0f) skyG = 1.0f;
        if (skyB < 0.0f) skyB = 0.0f;
        if (skyB > 1.0f) skyB = 1.0f;



        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glClearColor(skyR, skyG, skyB, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (isDay) 
        {
            sun.render();
        }
        else 
        {
            updateStars(deltaTime, skyR, skyG, skyB);
            renderStars();
            moon.render();
        }

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

void createStars(int numberOfStars) {
    for (int i = 0; i < 15; ++i) 
    {
        float x = (rand() % 2000 - 1000) / 1000.0f;
        float y = (rand() % 1000) / 1000.0f;
        stars.push_back(std::make_unique<Star>(x, y));
    }
}

void updateStars(float deltaTime, float skyR, float skyG, float skyB) {
    // Calculate brightness based on the sky's color
    float brightness = (skyR + skyG + skyB) / 3.0f; // Adjust calculation as needed
    for (const auto& star : stars) {
        star->update(brightness);  // Use -> to call update on the actual Star
    }
}

void renderStars() {
    for (const auto& star : stars) {
        star->render();  
    }
}