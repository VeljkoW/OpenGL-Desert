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
#include "fish.h"
#include "oasisSurface.h"
#include "text.h"
#include "entrance.h"

std::vector<std::unique_ptr<Star>> stars;

void createStars(int numberOfStars);
void updateStars(float deltaTime, float skyR, float skyG, float skyB);
void renderStars();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
bool isPointInEntrance(float x, float y, const std::vector<std::pair<float, float>>& polygon);

float lastFrameTime = 0.0f;
const float FPS = 60.0f;
const float FRAME_TIME = 1.0f / FPS;

bool entranceClicked = false;

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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

    Fish fish = Fish(-0.5f,-0.48f);
    OasisSurface oasisSurface = OasisSurface();

    float aspectRatio = static_cast<float>(wWidth) / static_cast<float>(wHeight);

    createStars(15);


    Text NameAndIndex = Text("Movistar Text Regular.ttf", "text.vert", "text.frag", 20);

    Text EntranceText = Text("Movistar Text Regular.ttf", "text.vert", "text.frag", 10);
    
    float EntranceTextTimer = 0.0f;

    Entrance entrance1(0.35f, 0.4f, 0.5f, -0.1f);
    Entrance entrance2(0.30f, 0.32f, 0.8f, -0.3f);
    Entrance entrance3(0.27f, 0.3f, 0.46f, -0.5f);

    while (!glfwWindowShouldClose(window)) 
    {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        float frameTime = FRAME_TIME;

        if (deltaTime < frameTime) {
            glfwWaitEventsTimeout(frameTime - deltaTime);
            currentFrameTime = glfwGetTime();
            deltaTime = currentFrameTime - lastFrameTime;

        }
        lastFrameTime = currentFrameTime;


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
            oasisSurface.setAlpha(0.5f);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            grassBed.setAlpha(1.0f);
            grassBlades.setAlpha(1.0f);
            oasisSurface.setAlpha(1.0f);
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
        fish.updatePosition(deltaTime);
        fish.render();
        oasisSurface.render();

        grassBlades.render();

        pyramid1.render(window,true);
        pyramid2.render(window,false);
        pyramid3.render(window,false);


        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            entrance1.setAnimationStarted(true);
            entrance2.setAnimationStarted(true);
            entrance3.setAnimationStarted(true);
        }

        entrance1.render();
        entrance2.render();
        entrance3.render();

        NameAndIndex.RenderText("Veljko Vulin RA69/2021", 0.0f, 10.0f, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));

        if (entranceClicked)
        {
            EntranceTextTimer += 0.05f;
            //Nastaviće
            EntranceText.RenderText("N", 0.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            if (EntranceTextTimer > 1.0f)
            {
                EntranceText.RenderText("a", 9.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 2.0f)
            {
                EntranceText.RenderText("s", 18.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 3.0f)
            {
                EntranceText.RenderText("t", 27.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 4.0f)
            {
                EntranceText.RenderText("a", 36.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 5.0f)
            {
                EntranceText.RenderText("v", 45.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 6.0f)
            {
                EntranceText.RenderText("i", 54.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 7.0f)
            {
                EntranceText.RenderText("ć", 63.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 8.0f)
            {
                EntranceText.RenderText("e", 72.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }

            //se
            if (EntranceTextTimer > 9.0f)
            {
                EntranceText.RenderText("s", 90.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 10.0f)
            {
                EntranceText.RenderText("e", 99.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            //na
            if (EntranceTextTimer > 11.0f)
            {
                EntranceText.RenderText("n", 117.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 12.0f)
            {
                EntranceText.RenderText("a", 126.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            //3D
            if (EntranceTextTimer > 13.0f)
            {
                EntranceText.RenderText("3", 144.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 14.0f)
            {
                EntranceText.RenderText("D", 153.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            //projektu
            if (EntranceTextTimer > 15.0f)
            {
                EntranceText.RenderText("p", 171.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 16.0f)
            {
                EntranceText.RenderText("r", 180.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 17.0f)
            {
                EntranceText.RenderText("o", 189.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 18.0f)
            {
                EntranceText.RenderText("j", 198.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 19.0f)
            {
                EntranceText.RenderText("e", 207.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 20.0f)
            {
                EntranceText.RenderText("k", 216.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 21.0f)
            {
                EntranceText.RenderText("t", 225.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (EntranceTextTimer > 22.0f)
            {
                EntranceText.RenderText("u", 234.0f, 500.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
            }

            
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos); // Get the mouse cursor position

        // Convert the mouse coordinates to OpenGL coordinates
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        xpos = (xpos / windowWidth) * 2.0f - 1.0f; // Convert to OpenGL coordinates (-1 to 1 range)
        ypos = -((ypos / windowHeight) * 2.0f - 1.0f); // Convert to OpenGL coordinates (-1 to 1 range)

        // Print the mouse position in OpenGL coordinates
        std::cout << "Mouse Clicked at OpenGL Coordinates: (" << xpos << ", " << ypos << ")\n";


        std::vector<std::pair<float, float>> entrance1Vertices = {
            {0.370833f, -0.118519f},  // Bottom-left
            {0.455208f, -0.159259f},  // Bottom-right
            {0.455208f,  0.0333333f}, // Top-right
            {0.429167f,  0.0481481f}  // Top-left
        };
        std::vector<std::pair<float, float>> entrance2Vertices = {
            {0.689583f, -0.316667f},  // Bottom-left
            {0.761458f, -0.353704f},  // Bottom-right
            {0.7625f,   -0.198148f},  // Top-right
            {0.747917f, -0.188889f}   // Top-left
        };
        std::vector<std::pair<float, float>> entrance3Vertices = {
            {0.360417f, -0.516667f},  // Bottom-left
            {0.425f,    -0.544444f},  // Bottom-right
            {0.426042f, -0.401852f},  // Top-right
            {0.41875f,  -0.398148f}   // Top-left
        };

        if (isPointInEntrance(xpos, ypos, entrance1Vertices)) {
            entranceClicked = true;
            std::cout << "Entrance 1 clicked!\n";
        }
        else if (isPointInEntrance(xpos, ypos, entrance2Vertices)) {
            entranceClicked = true;
            std::cout << "Entrance 2 clicked!\n";
        }
        else if (isPointInEntrance(xpos, ypos, entrance3Vertices)) {
            entranceClicked = true;
            std::cout << "Entrance 3 clicked!\n";
        }

    }
}
bool isPointInEntrance(float x, float y, const std::vector<std::pair<float, float>>& polygon)   //RAY CASTING ALGORITHM
{
    int intersectionCount = 0; // Count of intersections with polygon edges
    int vertexCount = polygon.size();

    for (int i = 0; i < vertexCount; ++i) {
        // Current vertex and the next vertex (wrapping around at the end)
        float x1 = polygon[i].first;
        float y1 = polygon[i].second;
        float x2 = polygon[(i + 1) % vertexCount].first;
        float y2 = polygon[(i + 1) % vertexCount].second;

        // Check if the ray intersects with the edge
        if (((y1 > y) != (y2 > y)) &&  // Check if the y-coordinate of the point is within the vertical range of the edge
            (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)) { // Calculate intersection point's x and check if it's to the right of the point
            intersectionCount++;
        }
    }

    // If the number of intersections is odd, the point is inside
    return (intersectionCount % 2) == 1;
}