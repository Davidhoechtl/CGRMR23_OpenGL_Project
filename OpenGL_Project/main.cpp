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

Camera2D camera(0.0f, 0.0f, 1.0f, 300.0f);

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
    //GLenum code = errorCheck();

    //configure viewport and set callback function for resizing
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    // setup shader
    TextRenderer textRenderer("fonts/arial.ttf", 48);
    Shader ourShader("Custom_Shaders/testVertexShader.txt", "Custom_Shaders/testFragmentShader.txt");
    Shader noTextureShader("Custom_Shaders/testVertexShader.txt", "Custom_Shaders/noTextureFragmentShader.txt");
    unsigned int texture = imageLoader.loadImage("Ressources/container.jpg", false);
    unsigned int roofTexture = imageLoader.loadImage("Ressources/BlueRoof.png", true);
    unsigned int coinTexture = imageLoader.loadTransparentImage("Ressources/coin.png", true);
    unsigned int testTexture = imageLoader.loadTileSetTexture("Ressources/TileSet.png", false);
    unsigned int fenceTexture = imageLoader.loadTransparentImage("Ressources/fence.png", false);

    // x = 200, y = 200, width = 150, height = 200
    Rectangle test(300.0f, 400.0f, 150.0f, 150.0f);
    test.SetTexture(texture);

    //Rectangle test2(0.0f, -1.0f, 100.0f, 100.0f);
    //test2.SetTexture(texture);

    Triangle roof(300.0f, 525.0f, 150.0f, 100.0f);
    roof.SetTexture(roofTexture);

    Rectangle coin(100.0f, 100.0f, 50.0f, 50.0f);
    coin.SetTexture(coinTexture);

    //FARMING HOUSE
    Rectangle farmingHouse1(-600.0f, 600.0f, 170.0f, 120.0f);
    Rectangle farmingHouse2(-600.0f, 690.0f, 120.0f, 60.0f);
    Triangle farmingHouse3(-600.0f, 740.0f, 120.0f, 40.0f);
    Rectangle farmingHouse4(-658.0f, 685.0f, 65.0f, 30.0f);
    //farmingHouse4.SetTexture(texture);
    farmingHouse4.SetRotationZAxis(67);
    Rectangle farmingHouse5(-542.0f, 685.0f, 65.0f, 30.0f);
    //farmingHouse5.SetTexture(texture);
    farmingHouse5.SetRotationZAxis(293);

    Rectangle farmingHouseRoof1(-630.0f, 740.0f, 90.0f, 10.0f);
    farmingHouseRoof1.SetTexture(texture);
    farmingHouseRoof1.SetRotationZAxis(35);

    Rectangle farmingHouseRoof2(-570.0f, 740.0f, 90.0f, 10.0f);
    farmingHouseRoof2.SetTexture(texture);
    farmingHouseRoof2.SetRotationZAxis(145);

    Rectangle farmingHouseRoof3(-675.0f, 685.0f, 70.0f, 10.0f);
    farmingHouseRoof3.SetTexture(texture);
    farmingHouseRoof3.SetRotationZAxis(70);

    Rectangle farmingHouseRoof4(-525.0f, 685.0f, 70.0f, 10.0f);
    farmingHouseRoof4.SetTexture(texture);
    farmingHouseRoof4.SetRotationZAxis(110);

    Rectangle farmingHouseDoor1(-650.0f, 590.0f, 100.0f, 5.0f);
    farmingHouseDoor1.SetRotationZAxis(90);
    farmingHouseDoor1.SetShader(&noTextureShader);
    farmingHouseDoor1.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseDoor2(-550.0f, 590.0f, 100.0f, 5.0f);
    farmingHouseDoor2.SetRotationZAxis(90);
    farmingHouseDoor2.SetShader(&noTextureShader);
    farmingHouseDoor2.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseDoor3(-600.0f, 544.0f, 105.0f, 7.0f);
    farmingHouseDoor3.SetShader(&noTextureShader);
    farmingHouseDoor3.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseDoor4(-600.0f, 638.0f, 105.0f, 7.0f);
    farmingHouseDoor4.SetShader(&noTextureShader);
    farmingHouseDoor4.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseDoor5(-601.0f, 590.0f, 131.0f, 5.0f);
    farmingHouseDoor5.SetRotationZAxis(42);
    farmingHouseDoor5.SetShader(&noTextureShader);
    farmingHouseDoor5.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseDoor6(-599.0f, 590.0f, 131.0f, 5.0f);
    farmingHouseDoor6.SetRotationZAxis(138);
    farmingHouseDoor6.SetShader(&noTextureShader);
    farmingHouseDoor6.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseWindow1(-600.0f, 690.0f, 30.0f, 40.0f);
    farmingHouseWindow1.SetShader(&noTextureShader);
    farmingHouseWindow1.SetColor(0.2f, 0.16f, 0.16f);

    Rectangle farmingHouseWindow2(-600.0f, 668.0f, 35.0f, 5.0f);
    farmingHouseWindow2.SetShader(&noTextureShader);
    farmingHouseWindow2.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseWindow3(-600.0f, 712.0f, 44.0f, 5.0f);
    farmingHouseWindow3.SetShader(&noTextureShader);
    farmingHouseWindow3.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseWindow4(-615.0f, 690.0f, 40.0f, 5.0f);
    farmingHouseWindow4.SetRotationZAxis(90);
    farmingHouseWindow4.SetShader(&noTextureShader);
    farmingHouseWindow4.SetColor(0.88f, 0.78f, 0.66f);

    Rectangle farmingHouseWindow5(-585.0f, 690.0f, 40.0f, 5.0f);
    farmingHouseWindow5.SetRotationZAxis(90);
    farmingHouseWindow5.SetShader(&noTextureShader);
    farmingHouseWindow5.SetColor(0.88f, 0.78f, 0.66f);

    farmingHouse1.SetShader(&noTextureShader);
    farmingHouse2.SetShader(&noTextureShader);
    farmingHouse3.SetShader(&noTextureShader);
    farmingHouse4.SetShader(&noTextureShader);
    farmingHouse5.SetShader(&noTextureShader);

    farmingHouse1.SetColor(0.68f, 0.1f, 0.0f);
    farmingHouse2.SetColor(0.68f, 0.1f, 0.0f);
    farmingHouse3.SetColor(0.68f, 0.1f, 0.0f);
    farmingHouse4.SetColor(0.68f, 0.1f, 0.0f);
    farmingHouse5.SetColor(0.68f, 0.1f, 0.0f);

    Rectangle fence1(-465.0f, 300.0f, 100.0f, 50.0f);
    fence1.SetTexture(fenceTexture);
    Rectangle fence2(-560.0f, 300.0f, 100.0f, 50.0f);
    fence2.SetTexture(fenceTexture);
    Rectangle fence3(-655.0f, 300.0f, 100.0f, 50.0f);
    fence3.SetTexture(fenceTexture);
    Rectangle fence4(-750.0f, 300.0f, 100.0f, 50.0f);
    fence4.SetTexture(fenceTexture);

    Rectangle fence5(-240.0f, 370.0f, 100.0f, 50.0f);
    fence5.SetRotationZAxis(-90);
    fence5.SetTexture(fenceTexture);
    Rectangle fence6(-240.0f, 465.0f, 100.0f, 50.0f);
    fence6.SetRotationZAxis(-90);
    fence6.SetTexture(fenceTexture);
    Rectangle fence7(-240.0f, 560.0f, 100.0f, 50.0f);
    fence7.SetRotationZAxis(-90);
    fence7.SetTexture(fenceTexture);
    Rectangle fence8(-240.0f, 655.0f, 100.0f, 50.0f);
    fence8.SetRotationZAxis(-90);
    fence8.SetTexture(fenceTexture);
    Rectangle fence9(-240.0f, 750.0f, 100.0f, 50.0f);
    fence9.SetRotationZAxis(-90);
    fence9.SetTexture(fenceTexture);


    //TileMap (Floor)
    TileMap tileMap(50, 50);

    for (int i = 0; i < tileMap.GetSize(); i++)
    {
        tileMap.Tiles[i] = (unsigned char)rand() % 4;
    }
    tileMap.TileSetHandle = testTexture;
    TileMapRenderer tileMapRenderer(tileMap);
    
    gameObjects.push_back(&test);
    gameObjects.push_back(&roof);
    gameObjects.push_back(&coin);
    gameObjects.push_back(&farmingHouse1);
    gameObjects.push_back(&farmingHouse2);
    gameObjects.push_back(&farmingHouse3);
    gameObjects.push_back(&farmingHouse4);
    gameObjects.push_back(&farmingHouse5);
    gameObjects.push_back(&farmingHouseRoof1);
    gameObjects.push_back(&farmingHouseRoof2);
    gameObjects.push_back(&farmingHouseRoof3);
    gameObjects.push_back(&farmingHouseRoof4);
    gameObjects.push_back(&farmingHouseDoor1);
    gameObjects.push_back(&farmingHouseDoor2);
    gameObjects.push_back(&farmingHouseDoor3);
    gameObjects.push_back(&farmingHouseDoor4);
    gameObjects.push_back(&farmingHouseDoor5);
    gameObjects.push_back(&farmingHouseDoor6);
    gameObjects.push_back(&farmingHouseWindow1);
    gameObjects.push_back(&farmingHouseWindow2);
    gameObjects.push_back(&farmingHouseWindow3);
    gameObjects.push_back(&farmingHouseWindow4);
    gameObjects.push_back(&farmingHouseWindow5);
    gameObjects.push_back(&fence1);
    gameObjects.push_back(&fence2);
    gameObjects.push_back(&fence3);
    gameObjects.push_back(&fence4);
    gameObjects.push_back(&fence5);
    gameObjects.push_back(&fence6);
    gameObjects.push_back(&fence7);
    gameObjects.push_back(&fence8);
    gameObjects.push_back(&fence9);

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

        glm::mat4 tileMapProjectionMatrix = tileMapRenderer.GetProjectionMatrix(viewportWidth, viewportHeight, camera.X, camera.Y);
        tileMapRenderer.Render(tileMapProjectionMatrix);

        for_each(gameObjects.begin(), gameObjects.end(), [projectionMatrix](GameObject* obj) {
            obj->Render(projectionMatrix);
        });

        glm::mat4 textProjectionMatrix = glm::ortho(0.0f, static_cast<float>(viewportWidth), 0.0f, static_cast<float>(viewportHeight));
        textRenderer.RenderText("Coins: 0/1", 620.0f, 550.0f, 0.7f, glm::vec3(1, 1.0f, 1.0f), textProjectionMatrix);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
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
