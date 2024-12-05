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


    Text NameAndIndex = Text("Movistar Text Regular.ttf", 20);

    std::string fontPath = "Korela Serif.otf";
    int fontSize = 17;
    //Let me cook
    Text N = Text(fontPath, fontSize);
    Text a = Text(fontPath, fontSize);
    Text s = Text(fontPath, fontSize);
    Text t = Text(fontPath, fontSize);
    Text a2 = Text(fontPath, fontSize);
    Text v = Text(fontPath, fontSize);
    Text i = Text(fontPath, fontSize);
    Text c = Text(fontPath, fontSize);
    Text zarez = Text(fontPath, fontSize);
    Text e = Text(fontPath, fontSize);
    Text s2 = Text(fontPath, fontSize);
    Text e2 = Text(fontPath, fontSize);
    Text n = Text(fontPath, fontSize);
    Text a3 = Text(fontPath, fontSize);
    Text tri = Text(fontPath, fontSize);
    Text D = Text(fontPath, fontSize);
    Text p = Text(fontPath, fontSize);
    Text r = Text(fontPath, fontSize);
    Text o = Text(fontPath, fontSize);
    Text j = Text(fontPath, fontSize);
    Text e3 = Text(fontPath, fontSize);
    Text k = Text(fontPath, fontSize);
    Text t2 = Text(fontPath, fontSize);
    Text u = Text(fontPath, fontSize);

    glm::vec3 textColor(1.0f, 0.0f, 0.0f);

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

            if (sun.getPosX() > -0.70f - sun.getRadius())
            {
                skyR = std::max(0.0f, skyR - 0.003f);
                skyG = std::min(0.6f, skyG + 0.003f);
                skyB = std::min(1.0f, skyB + 0.003f);
            }
            else if (sun.getPosX() > -0.97f - sun.getRadius())
            {
                skyR = std::min(247.0f / 255.0f, skyR + 0.007f);  
                skyG = std::min(147.0f / 255.0f, skyG + 0.006f);  
                skyB = std::max(114.0f / 255.0f, skyB - 0.006f);
            }
            else
            {
                skyR = std::max(0.0f, skyR - 0.007f);
                skyG = std::max(0.0f, skyG - 0.006f);
                skyB = std::max(0.0f, skyB - 0.006f);
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
                skyR = std::max(0.0f, skyR - 0.007f);
                skyG = std::max(0.0f, skyG - 0.006f);
                skyB = std::max(0.0f, skyB - 0.006f);
            }
            else
            {
                skyR = std::max(0.0f, skyR - 0.003f);
                skyG = std::min(0.6f, skyG + 0.003f);
                skyB = std::min(1.0f, skyB + 0.003f);
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

        NameAndIndex.Render("Veljko Vulin RA69/2021", 0.0f, 10.0f, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));

        if (entrance1.getProgress() != 1.0f && entranceClicked)
        {
            entranceClicked = false;
        }

        //Spaghetti code warning
        if (entranceClicked && entrance1.getProgress() == 1.0f)
        {
            EntranceTextTimer += 0.05f;
            //Nastaviće
            N.Render("N", 250.0f, 500.0f, 1.0f, textColor);
            if (EntranceTextTimer > 24.0f && N.getAlpha() > 0.0f)
            {
                N.reduceAlpha(0.05f);
            }
            if (EntranceTextTimer > 1.0f)
            {
                a.Render("a", 260.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 25.0f && a.getAlpha() > 0.0f)
                {
                    a.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 2.0f)
            {
                s.Render("s", 270.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 26.0f && s.getAlpha() > 0.0f)
                {
                    s.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 3.0f)
            {
                t.Render("t", 280.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 27.0f && t.getAlpha() > 0.0f)
                {
                    t.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 4.0f)
            {
                a2.Render("a", 290.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 28.0f && a2.getAlpha() > 0.0f)
                {
                    a2.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 5.0f)
            {
                v.Render("v", 300.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 29.0f && v.getAlpha() > 0.0f)
                {
                    v.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 6.0f)
            {
                i.Render("i", 310.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 30.0f && i.getAlpha() > 0.0f)
                {
                    i.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 7.0f)
            {
                c.Render("c", 320.0f, 500.0f, 1.0f, textColor);
                zarez.Render(",", 322.0f, 511.0f, 1.0f, textColor);
                if (EntranceTextTimer > 31.0f && c.getAlpha() > 0.0f && zarez.getAlpha() > 0.0f)
                {
                    c.reduceAlpha(0.05f);
                    zarez.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 8.0f)
            {
                e.Render("e", 330.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 32.0f && e.getAlpha() > 0.0f)
                {
                    e.reduceAlpha(0.05f);
                }
            }

            //se
            if (EntranceTextTimer > 9.0f)
            {
                s2.Render("s", 350.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 33.0f && s2.getAlpha() > 0.0f)
                {
                    s2.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 10.0f)
            {
                e2.Render("e", 360.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 34.0f && e2.getAlpha() > 0.0f)
                {
                    e2.reduceAlpha(0.05f);
                }
            }
            //na
            if (EntranceTextTimer > 11.0f)
            {
                n.Render("n", 380.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 35.0f && n.getAlpha() > 0.0f)
                {
                    n.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 12.0f)
            {
                a3.Render("a", 390.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 36.0f && a3.getAlpha() > 0.0f)
                {
                    a3.reduceAlpha(0.05f);
                }
            }
            //3D
            if (EntranceTextTimer > 13.0f)
            {
                tri.Render("3", 410.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 37.0f && tri.getAlpha() > 0.0f)
                {
                    tri.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 14.0f)
            {
                D.Render("D", 420.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 38.0f && D.getAlpha() > 0.0f)
                {
                    D.reduceAlpha(0.05f);
                }
            }
            //projektu
            if (EntranceTextTimer > 15.0f)
            {
                p.Render("p", 440.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 39.0f && p.getAlpha() > 0.0f)
                {
                    p.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 16.0f)
            {
                r.Render("r", 450.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 40.0f && r.getAlpha() > 0.0f)
                {
                    r.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 17.0f)
            {
                o.Render("o", 460.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 41.0f && o.getAlpha() > 0.0f)
                {
                    o.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 18.0f)
            {
                j.Render("j", 470.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 42.0f && j.getAlpha() > 0.0f)
                {
                    j.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 19.0f)
            {
                e3.Render("e", 480.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 43.0f && e3.getAlpha() > 0.0f)
                {
                    e3.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 20.0f)
            {
                k.Render("k", 490.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 44.0f && k.getAlpha() > 0.0f)
                {
                    k.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 21.0f)
            {
                t2.Render("t", 500.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 45.0f && t2.getAlpha() > 0.0f)
                {
                    t2.reduceAlpha(0.05f);
                }
            }
            if (EntranceTextTimer > 22.0f)
            {
                u.Render("u", 509.0f, 500.0f, 1.0f, textColor);
                if (EntranceTextTimer > 46.0f && u.getAlpha() > 0.0f)
                {
                    u.reduceAlpha(0.05f);
                }
            }

            //The exodia
            if (EntranceTextTimer > 50.0f)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
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
        glfwGetCursorPos(window, &xpos, &ypos);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        xpos = (xpos / windowWidth) * 2.0f - 1.0f;
        ypos = -((ypos / windowHeight) * 2.0f - 1.0f);

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