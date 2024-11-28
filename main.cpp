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
#include "oasis.h"
#include "grassBlades.h"
#include "grassBed.h"

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
    
    GrassBed grassBed = GrassBed();
    Oasis oasis = Oasis();
    GrassBlades grassBlades = GrassBlades();

    float aspectRatio = static_cast<float>(wWidth) / static_cast<float>(wHeight);

    createStars(15);

    while (!glfwWindowShouldClose(window)) 
    {
        float deltaTime = calculateDeltaTime();
        if (isDay && sun.getSpeed() != 0.0f) {
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
        else if (moon.getSpeed() != 0.0f)
        {
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
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            sun.setSpeed(0.0f);
            moon.setSpeed(0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            sun.setSpeed(0.1f);
            moon.setSpeed(0.1f);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            isDay = true;
            sun.setSpeed(0.1f);
            moon.setSpeed(0.1f);
            skyR = 0.0f, 
            skyG = 0.6f, 
            skyB = 1.0f;
            sun.setPosX(0.0f);
            sun.setPosY(0.6f);
            moon.setPosX(1.0f + moon.getRadius());
            moon.setPosY(0.6f);
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            grassBed.setAlpha(0.0f);
            grassBlades.setAlpha(0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            grassBed.setAlpha(1.0f);
            grassBlades.setAlpha(1.0f);
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
        grassBed.render();
        oasis.render();
        grassBlades.render();

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
    static float lastFrameTime = 0.0f;
    float currentFrameTime = glfwGetTime();
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    return deltaTime; 
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
    float brightness = (skyR + skyG + skyB) / 3.0f;
    for (const auto& star : stars) {
        star->update(brightness); 
    }
}

void renderStars() {
    for (const auto& star : stars) {
        star->render();  
    }
}