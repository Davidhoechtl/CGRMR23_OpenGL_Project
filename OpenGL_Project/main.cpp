#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Custom_Shaders/Shader.h"
#include "ImageLoader/ImageLoader.h" 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Game/Objects/Rectangle.h"
#include "Game/Objects/Triangle.h"
#include "Game/Camera.h"

using namespace std;

//Forward Declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float delta);
void SetupTriangle();

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
    Shader ourShader("Custom_Shaders/testVertexShader.txt", "Custom_Shaders/testFragmentShader.txt");
    unsigned int texture = imageLoader.loadImage("Ressources/container.jpg");
    unsigned int roofTexture = imageLoader.loadImage("Ressources/BlueRoof.png");
    // x = 200, y = 200, width = 150, height = 200
    Rectangle test(0.0f, 0.0f, 150.0f, 150.0f);
    test.SetTexture(texture);

    //Rectangle test2(0.0f, -1.0f, 100.0f, 100.0f);
    //test2.SetTexture(texture);

    Triangle roof(0.0f, 125.0f, 150.0f, 100.0f);
    roof.SetTexture(roofTexture);
    //Shader triangleShader("Custom_Shaders/triangleVS.txt", "Custom_Shaders/triangleFS.txt");

    gameObjects.push_back(&test);
    //gameObjects.push_back(&test2);
    gameObjects.push_back(&roof);

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

void SetupTriangle() {
    // vertecies of the triangle (z = 0.0f -> no depth so it appears 2D)
    float vertices[] = {
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // top left
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top right
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // setup Vertex Buffer Object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBO; // Element Buffer Object (EBO)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}