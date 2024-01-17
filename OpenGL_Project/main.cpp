#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Custom_Shaders/Shader.h"
#include "ImageLoader/ImageLoader.h" 
#include "TextRenderer/TextRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Game/Objects/Rectangle.h"
#include "Game/Objects/Triangle.h"
#include "Game/Camera.h"

#include "TileMapRenderer/TileMap.h";
#include "TileMapRenderer/TileMapRenderer.h"

using namespace std;

//Forward Declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float delta);
GLenum errorCheck();

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Camera2D camera(0.0f, 0.0f, 1.0f, 100.0f);

// game objects
vector<GameObject*> gameObjects;

int main()
{
    ImageLoader imageLoader;
    
    //init glfw (OpenGL Version: 4.6, Core Profile)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Name TBD", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    //configure viewport and set callback function for resizing
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);

    // setup shader
    TextRenderer textRenderer("fonts/arial.ttf", 48);
    Shader ourShader("Custom_Shaders/testVertexShader.txt", "Custom_Shaders/testFragmentShader.txt");
    unsigned int texture = imageLoader.loadImage("Ressources/container.jpg", false);
    unsigned int roofTexture = imageLoader.loadImage("Ressources/BlueRoof.png", true);
    unsigned int coinTexture = imageLoader.loadTransparentImage("Ressources/coin.png", true);
    
    // x = 200, y = 200, width = 150, height = 200
    Rectangle test(0.0f, 0.0f, 150.0f, 150.0f);
    test.SetTexture(texture);

    //Rectangle test2(0.0f, -1.0f, 100.0f, 100.0f);
    //test2.SetTexture(texture);

    Triangle roof(0.0f, 125.0f, 150.0f, 100.0f);
    roof.SetTexture(roofTexture);

    Rectangle coin(100.0f, 100.0f, 50.0f, 50.0f);
    coin.SetTexture(coinTexture);

    //Map
    TileMap tileMap(64, 64);

    for (int i = 0; i < tileMap.GetSize(); i++)
    {
        tileMap.Tiles[i] = (unsigned char)rand() % 4;
    }
    tileMap.TileSetHandle = imageLoader.loadImage("Ressources/container.jpg", false);
    TileMapRenderer tileMapRenderer(tileMap);
    
    gameObjects.push_back(&test);
    //gameObjects.push_back(&test2);
    gameObjects.push_back(&roof);
    gameObjects.push_back(&coin);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //rendering loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for_each(gameObjects.begin(), gameObjects.end(), [](GameObject* obj) {
            obj->Update();
        });

        glm::mat4 projectionMatrix = camera.GetProjectionMatrix(viewportWidth, viewportHeight);

        for_each(gameObjects.begin(), gameObjects.end(), [projectionMatrix](GameObject* obj) {
            obj-> Render(projectionMatrix);
        });
        errorCheck();
        tileMapRenderer.Render(projectionMatrix);
        errorCheck();
        glm::mat4 textProjectionMatrix = glm::ortho(0.0f, static_cast<float>(viewportWidth), 0.0f, static_cast<float>(viewportHeight));

        textRenderer.RenderText("Coins: 0/1", 620.0f, 550.0f, 0.7f, glm::vec3(1, 1.0f, 1.0f), textProjectionMatrix);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    errorCheck();
    //cleans all GLFW ressources
    glfwTerminate();
    return 0;
}

//function to resize viewport, when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//handles all user input
void processInput(GLFWwindow* window, float delta)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Y += camera.CameraSpeed * delta;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Y -= camera.CameraSpeed * delta;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.X -= camera.CameraSpeed * delta;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.X += camera.CameraSpeed * delta;
}

GLenum errorCheck()
{
    GLenum code;
    const GLubyte* string;
    code = glGetError();
    
    return code;
}
