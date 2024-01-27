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
#include "Game/Objects/Characters/Player.h"

#include "TileMapRenderer/TileMap.h";
#include "TileMapRenderer/TileMapRenderer.h"
#include "Game/Utils/StaticGameInfo.h"

using namespace std;

//Forward Declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float delta);
GLenum errorCheck();
void createRoad(TileMap* tileMap);
void createRoad3x3(TileMap* tileMap, int centerX, int centerY);
void createFarmField(TileMap* tileMap, int x, int y, int width, int height);

// timing
static float GameTime = 0;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Player* player = nullptr;

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
	unsigned int coinTexture = imageLoader.loadTransparentImage("Ressources/coin.png", true);
	unsigned int testTexture = imageLoader.loadTileSetTexture("Ressources/TileSet.png", false);
	unsigned int fenceTexture = imageLoader.loadTransparentImage("Ressources/fence.png", false);
	unsigned int clockTexture = imageLoader.loadTransparentImage("Ressources/clock.png", false);
	unsigned int rockWallTexture = imageLoader.loadImage("Ressources/219.jpg", false);
	unsigned int churchRoofTexture = imageLoader.loadImage("Ressources/churchRoof.png", false);
	unsigned int chimneyTexture = imageLoader.loadImage("Ressources/202.jpg", false);
	unsigned int doorTexture = imageLoader.loadImage("Ressources/Door_01.png", true);
	unsigned int treeTexture = imageLoader.loadImage("Ressources/Tree.png", true);
	unsigned int waterTexture = imageLoader.loadImage("Ressources/Water.jpg", true);

	Rectangle coin(100.0f, 100.0f, 50.0f, 50.0f);
	coin.SetTexture(coinTexture);

	player = new Player(200, 200);

	//FARMING HOUSE
	Rectangle farmingHouse1(-600.0f, 430.0f, 170.0f, 120.0f);
	Rectangle farmingHouse2(-600.0f, 520.0f, 120.0f, 60.0f);
	Triangle farmingHouse3(-600.0f, 570.0f, 120.0f, 40.0f);
	Rectangle farmingHouse4(-658.0f, 515.0f, 65.0f, 30.0f);
	//farmingHouse4.SetTexture(texture);
	farmingHouse4.SetRotationZAxis(67);
	Rectangle farmingHouse5(-542.0f, 515.0f, 65.0f, 30.0f);
	//farmingHouse5.SetTexture(texture);
	farmingHouse5.SetRotationZAxis(293);

	Rectangle farmingHouseRoof1(-630.0f, 570.0f, 90.0f, 10.0f);
	farmingHouseRoof1.SetTexture(texture);
	farmingHouseRoof1.SetRotationZAxis(35);

	Rectangle farmingHouseRoof2(-570.0f, 570.0f, 90.0f, 10.0f);
	farmingHouseRoof2.SetTexture(texture);
	farmingHouseRoof2.SetRotationZAxis(145);

	Rectangle farmingHouseRoof3(-675.0f, 515.0f, 70.0f, 10.0f);
	farmingHouseRoof3.SetTexture(texture);
	farmingHouseRoof3.SetRotationZAxis(70);

	Rectangle farmingHouseRoof4(-525.0f, 515.0f, 70.0f, 10.0f);
	farmingHouseRoof4.SetTexture(texture);
	farmingHouseRoof4.SetRotationZAxis(110);

	Rectangle farmingHouseDoor1(-650.0f, 420.0f, 100.0f, 5.0f);
	farmingHouseDoor1.SetRotationZAxis(90);
	farmingHouseDoor1.SetShader(&noTextureShader);
	farmingHouseDoor1.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseDoor2(-550.0f, 420.0f, 100.0f, 5.0f);
	farmingHouseDoor2.SetRotationZAxis(90);
	farmingHouseDoor2.SetShader(&noTextureShader);
	farmingHouseDoor2.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseDoor3(-600.0f, 373.0f, 105.0f, 7.0f);
	farmingHouseDoor3.SetShader(&noTextureShader);
	farmingHouseDoor3.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseDoor4(-600.0f, 468.0f, 105.0f, 7.0f);
	farmingHouseDoor4.SetShader(&noTextureShader);
	farmingHouseDoor4.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseDoor5(-601.0f, 420.0f, 131.0f, 5.0f);
	farmingHouseDoor5.SetRotationZAxis(42);
	farmingHouseDoor5.SetShader(&noTextureShader);
	farmingHouseDoor5.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseDoor6(-599.0f, 420.0f, 131.0f, 5.0f);
	farmingHouseDoor6.SetRotationZAxis(138);
	farmingHouseDoor6.SetShader(&noTextureShader);
	farmingHouseDoor6.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseWindow1(-600.0f, 520.0f, 30.0f, 40.0f);
	farmingHouseWindow1.SetShader(&noTextureShader);
	farmingHouseWindow1.SetColor(0.2f, 0.16f, 0.16f);

	Rectangle farmingHouseWindow2(-600.0f, 498.0f, 35.0f, 5.0f);
	farmingHouseWindow2.SetShader(&noTextureShader);
	farmingHouseWindow2.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseWindow3(-600.0f, 542.0f, 44.0f, 5.0f);
	farmingHouseWindow3.SetShader(&noTextureShader);
	farmingHouseWindow3.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseWindow4(-615.0f, 520.0f, 40.0f, 5.0f);
	farmingHouseWindow4.SetRotationZAxis(90);
	farmingHouseWindow4.SetShader(&noTextureShader);
	farmingHouseWindow4.SetColor(0.88f, 0.78f, 0.66f);

	Rectangle farmingHouseWindow5(-585.0f, 520.0f, 40.0f, 5.0f);
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

	Rectangle fence1(-405.0f, 250.0f, 100.0f, 50.0f);
	fence1.SetTexture(fenceTexture);
	Rectangle fence2(-500.0f, 250.0f, 100.0f, 50.0f);
	fence2.SetTexture(fenceTexture);
	Rectangle fence3(-595.0f, 250.0f, 100.0f, 50.0f);
	fence3.SetTexture(fenceTexture);
	Rectangle fence4(-690.0f, 250.0f, 100.0f, 50.0f);
	fence4.SetTexture(fenceTexture);

	Rectangle fence5(-240.0f, 400.0f, 100.0f, 50.0f);
	fence5.SetRotationZAxis(-90);
	fence5.SetTexture(fenceTexture);
	Rectangle fence6(-240.0f, 495.0f, 100.0f, 50.0f);
	fence6.SetRotationZAxis(-90);
	fence6.SetTexture(fenceTexture);
	Rectangle fence7(-240.0f, 590.0f, 100.0f, 50.0f);
	fence7.SetRotationZAxis(-90);
	fence7.SetTexture(fenceTexture);

	//CHURCH

	float churchX = -350;
	float churchY = -302;
	Rectangle church1(churchX, churchY, 70.0f, 100.0f);
	Rectangle church9(churchX, churchY + 100, 70.0f, 100.0f);
	church9.SetTexture(rockWallTexture);
	church1.SetTexture(rockWallTexture);
	Triangle church2(churchX, churchY + 170, 70.0f, 40.0f);
	church2.SetTexture(churchRoofTexture);
	Rectangle church3(churchX + 110, churchY, 150.0f, 100.0f);
	church3.SetTexture(rockWallTexture);
	Triangle church4(churchX + 110, churchY + 70, 150.0f, 40.0f);
	church4.SetTexture(churchRoofTexture);
	Rectangle church5(churchX, churchY + 110, 30.0f, 30.0f);
	church5.SetTexture(clockTexture);
	Rectangle church6(churchX + 110, churchY - 25, 30.0f, 50.0f);
	church6.SetShader(&noTextureShader);
	Rectangle church7(churchX, churchY + 203, 5.0f, 30.0f);
	Rectangle church8(churchX, churchY + 207, 5.0f, 20.0f);
	church8.SetRotationZAxis(90.0f);

	church2.SetColor(0.78f, 0.41f, 0.24f);
	church4.SetColor(0.78f, 0.41f, 0.24f);
	church6.SetColor(0.2f, 0.16f, 0.16f);

	//FARMER HOUSE
	float farmHouseX = 188;
	float farmHouseY = 498;
	Rectangle farmHouse1(farmHouseX, farmHouseY, 120.0f, 100.0f);
	farmHouse1.SetShader(&noTextureShader);
	Triangle farmHouse2(farmHouseX, farmHouseY + 80, 120.0f, 60.0f);
	farmHouse2.SetShader(&noTextureShader);
	Rectangle farmHouse3(farmHouseX - 30, farmHouseY - 20, 30.0f, 60.0f);
	farmHouse3.SetTexture(doorTexture);
	Rectangle farmHouse4(farmHouseX + 30, farmHouseY + 10, 30.0f, 30.0f);
	farmHouse4.SetShader(&noTextureShader);
	Rectangle farmHouse5(farmHouseX - 33, farmHouseY + 75, 100.0f, 5.0f);
	farmHouse5.SetRotationZAxis(45);
	farmHouse5.SetTexture(texture);
	Rectangle farmHouse6(farmHouseX + 35, farmHouseY + 75, 100.0f, 5.0f);
	farmHouse6.SetRotationZAxis(135);
	farmHouse6.SetTexture(texture);
	Rectangle farmHouse7(farmHouseX - 40, farmHouseY + 80, 25.0f, 45.0f);
	farmHouse7.SetTexture(chimneyTexture);
	Rectangle farmHouse8(farmHouseX - 40, farmHouseY + 102.5f, 35, 5);
	farmHouse8.SetTexture(chimneyTexture);
	Rectangle farmHouse9(farmHouseX + 30, farmHouseY + 25, 32, 3);
	farmHouse9.SetTexture(texture);
	Rectangle farmHouse10(farmHouseX + 30, farmHouseY - 5, 38, 5);
	farmHouse10.SetTexture(texture);
	Rectangle farmHouse11(farmHouseX + 15, farmHouseY + 10, 32, 3);
	farmHouse11.SetRotationZAxis(90);
	farmHouse11.SetTexture(texture);
	Rectangle farmHouse12(farmHouseX + 45, farmHouseY + 10, 32, 3);
	farmHouse12.SetRotationZAxis(90);
	farmHouse12.SetTexture(texture);
	Rectangle farmHouse13(farmHouseX + 30, farmHouseY + 10, 32, 3);
	farmHouse13.SetRotationZAxis(90);
	farmHouse13.SetTexture(texture);
	Rectangle farmHouse14(farmHouseX + 30, farmHouseY + 10, 32, 3);
	farmHouse14.SetTexture(texture);

	farmHouse8.SetColor(0.5f, 0.35f, 0.33f);
	farmHouse1.SetColor(0.92f, 0.89f, 0.87f);
	farmHouse2.SetColor(0.92f, 0.89f, 0.87f);
	farmHouse4.SetColor(0.58f, 0.78f, 0.9f);

	//VILLAGE
	float house1X = -50;
	float house1Y = -550;
	Rectangle house11(house1X, house1Y, 120.0f, 100.0f);
	house11.SetShader(&noTextureShader);
	Triangle house12(house1X, house1Y + 80, 120.0f, 60.0f);
	house12.SetShader(&noTextureShader);
	Rectangle house13(house1X - 30, house1Y - 20, 30.0f, 60.0f);
	house13.SetTexture(doorTexture);
	Rectangle house14(house1X + 30, house1Y + 10, 30.0f, 30.0f);
	house14.SetShader(&noTextureShader);
	Rectangle house15(house1X - 33, house1Y + 75, 100.0f, 5.0f);
	house15.SetRotationZAxis(45);
	house15.SetTexture(texture);
	Rectangle house16(house1X + 35, house1Y + 75, 100.0f, 5.0f);
	house16.SetRotationZAxis(135);
	house16.SetTexture(texture);
	Rectangle house17(house1X - 40, house1Y + 80, 25.0f, 45.0f);
	house17.SetTexture(chimneyTexture);
	Rectangle house18(house1X - 40, house1Y + 102.5f, 35, 5);
	house18.SetTexture(chimneyTexture);
	Rectangle house19(house1X + 30, house1Y + 25, 32, 3);
	house19.SetTexture(texture);
	Rectangle house110(house1X + 30, house1Y - 5, 38, 5);
	house110.SetTexture(texture);
	Rectangle house111(house1X + 15, house1Y + 10, 32, 3);
	house111.SetRotationZAxis(90);
	house111.SetTexture(texture);
	Rectangle house112(house1X + 45, house1Y + 10, 32, 3);
	house112.SetRotationZAxis(90);
	house112.SetTexture(texture);
	Rectangle house113(house1X + 30, house1Y + 10, 32, 3);
	house113.SetRotationZAxis(90);
	house113.SetTexture(texture);
	Rectangle house114(house1X + 30, house1Y + 10, 32, 3);
	house114.SetTexture(texture);

	house18.SetColor(0.5f, 0.35f, 0.33f);
	house11.SetColor(0.92f, 0.89f, 0.87f);
	house12.SetColor(0.92f, 0.89f, 0.87f);
	house14.SetColor(0.58f, 0.78f, 0.9f);

	float house2X = 150;
	float house2Y = -350;
	Rectangle house21(house2X, house2Y, 120.0f, 100.0f);
	house21.SetShader(&noTextureShader);
	Triangle house22(house2X, house2Y + 80, 120.0f, 60.0f);
	house22.SetShader(&noTextureShader);
	Rectangle house23(house2X - 30, house2Y - 20, 30.0f, 60.0f);
	house23.SetTexture(doorTexture);
	Rectangle house24(house2X + 30, house2Y + 10, 30.0f, 30.0f);
	house24.SetShader(&noTextureShader);
	Rectangle house25(house2X - 33, house2Y + 75, 100.0f, 5.0f);
	house25.SetRotationZAxis(45);
	house25.SetTexture(texture);
	Rectangle house26(house2X + 35, house2Y + 75, 100.0f, 5.0f);
	house26.SetRotationZAxis(135);
	house26.SetTexture(texture);
	Rectangle house27(house2X - 40, house2Y + 80, 25.0f, 45.0f);
	house27.SetTexture(chimneyTexture);
	Rectangle house28(house2X - 40, house2Y + 102.5f, 35, 5);
	house28.SetTexture(chimneyTexture);
	Rectangle house29(house2X + 30, house2Y + 25, 32, 3);
	house29.SetTexture(texture);
	Rectangle house210(house2X + 30, house2Y - 5, 38, 5);
	house210.SetTexture(texture);
	Rectangle house211(house2X + 15, house2Y + 10, 32, 3);
	house211.SetRotationZAxis(90);
	house211.SetTexture(texture);
	Rectangle house212(house2X + 45, house2Y + 10, 32, 3);
	house212.SetRotationZAxis(90);
	house212.SetTexture(texture);
	Rectangle house213(house2X + 30, house2Y + 10, 32, 3);
	house213.SetRotationZAxis(90);
	house213.SetTexture(texture);
	Rectangle house214(house2X + 30, house2Y + 10, 32, 3);
	house214.SetTexture(texture);

	house28.SetColor(0.5f, 0.35f, 0.33f);
	house21.SetColor(0.92f, 0.89f, 0.87f);
	house22.SetColor(0.92f, 0.89f, 0.87f);
	house24.SetColor(0.58f, 0.78f, 0.9f);

	float house3X = -550;
	float house3Y = -450;
	Rectangle house31(house3X, house3Y, 120.0f, 100.0f);
	house31.SetShader(&noTextureShader);
	Triangle house32(house3X, house3Y + 80, 120.0f, 60.0f);
	house32.SetShader(&noTextureShader);
	Rectangle house33(house3X - 30, house3Y - 20, 30.0f, 60.0f);
	house33.SetTexture(doorTexture);
	Rectangle house34(house3X + 30, house3Y + 10, 30.0f, 30.0f);
	house34.SetShader(&noTextureShader);
	Rectangle house35(house3X - 33, house3Y + 75, 100.0f, 5.0f);
	house35.SetRotationZAxis(45);
	house35.SetTexture(texture);
	Rectangle house36(house3X + 35, house3Y + 75, 100.0f, 5.0f);
	house36.SetRotationZAxis(135);
	house36.SetTexture(texture);
	Rectangle house37(house3X - 40, house3Y + 80, 25.0f, 45.0f);
	house37.SetTexture(chimneyTexture);
	Rectangle house38(house3X - 40, house3Y + 102.5f, 35, 5);
	house38.SetTexture(chimneyTexture);
	Rectangle house39(house3X + 30, house3Y + 25, 32, 3);
	house39.SetTexture(texture);
	Rectangle house310(house3X + 30, house3Y - 5, 38, 5);
	house310.SetTexture(texture);
	Rectangle house311(house3X + 15, house3Y + 10, 32, 3);
	house311.SetRotationZAxis(90);
	house311.SetTexture(texture);
	Rectangle house312(house3X + 45, house3Y + 10, 32, 3);
	house312.SetRotationZAxis(90);
	house312.SetTexture(texture);
	Rectangle house313(house3X + 30, house3Y + 10, 32, 3);
	house313.SetRotationZAxis(90);
	house313.SetTexture(texture);
	Rectangle house314(house3X + 30, house3Y + 10, 32, 3);
	house314.SetTexture(texture);

	house38.SetColor(0.5f, 0.35f, 0.33f);
	house31.SetColor(0.92f, 0.89f, 0.87f);
	house32.SetColor(0.92f, 0.89f, 0.87f);
	house34.SetColor(0.58f, 0.78f, 0.9f);

	float house4X = -500;
	float house4Y = -50;
	Rectangle house41(house4X, house4Y, 120.0f, 100.0f);
	house41.SetShader(&noTextureShader);
	Triangle house42(house4X, house4Y + 80, 120.0f, 60.0f);
	house42.SetShader(&noTextureShader);
	Rectangle house43(house4X - 30, house4Y - 20, 30.0f, 60.0f);
	house43.SetTexture(doorTexture);
	Rectangle house44(house4X + 30, house4Y + 10, 30.0f, 30.0f);
	house44.SetShader(&noTextureShader);
	Rectangle house45(house4X - 33, house4Y + 75, 100.0f, 5.0f);
	house45.SetRotationZAxis(45);
	house45.SetTexture(texture);
	Rectangle house46(house4X + 35, house4Y + 75, 100.0f, 5.0f);
	house46.SetRotationZAxis(135);
	house46.SetTexture(texture);
	Rectangle house47(house4X - 40, house4Y + 80, 25.0f, 45.0f);
	house47.SetTexture(chimneyTexture);
	Rectangle house48(house4X - 40, house4Y + 102.5f, 35, 5);
	house48.SetTexture(chimneyTexture);
	Rectangle house49(house4X + 30, house4Y + 25, 32, 3);
	house49.SetTexture(texture);
	Rectangle house410(house4X + 30, house4Y - 5, 38, 5);
	house410.SetTexture(texture);
	Rectangle house411(house4X + 15, house4Y + 10, 32, 3);
	house411.SetRotationZAxis(90);
	house411.SetTexture(texture);
	Rectangle house412(house4X + 45, house4Y + 10, 32, 3);
	house412.SetRotationZAxis(90);
	house412.SetTexture(texture);
	Rectangle house413(house4X + 30, house4Y + 10, 32, 3);
	house413.SetRotationZAxis(90);
	house413.SetTexture(texture);
	Rectangle house414(house4X + 30, house4Y + 10, 32, 3);
	house414.SetTexture(texture);

	house48.SetColor(0.5f, 0.35f, 0.33f);
	house41.SetColor(0.92f, 0.89f, 0.87f);
	house42.SetColor(0.92f, 0.89f, 0.87f);
	house44.SetColor(0.58f, 0.78f, 0.9f);

	//TREES
	float tree1X = 370;
	float tree1Y = -20;
	Rectangle tree11(tree1X, tree1Y, 20.0f, 50.0f);
	tree11.SetTexture(treeTexture);
	Triangle tree12(tree1X, tree1Y + 25, 60.0f, 50.0f);
	tree12.SetShader(&noTextureShader);
	Triangle tree13(tree1X, tree1Y + 45, 55.0f, 50.0f);
	tree13.SetShader(&noTextureShader);
	Triangle tree14(tree1X, tree1Y + 65, 50.0f, 50.0f);
	tree14.SetShader(&noTextureShader);
	tree12.SetColor(0.156f, 0.3f, 0.08f);
	tree13.SetColor(0.156f, 0.3f, 0.08f);
	tree14.SetColor(0.156f, 0.3f, 0.08f);

	float tree2X = 365;
	float tree2Y = -540;
	Rectangle tree21(tree2X, tree2Y, 20.0f, 50.0f);
	tree21.SetTexture(treeTexture);
	Triangle tree22(tree2X, tree2Y + 25, 60.0f, 50.0f);
	tree22.SetShader(&noTextureShader);
	Triangle tree23(tree2X, tree2Y + 45, 55.0f, 50.0f);
	tree23.SetShader(&noTextureShader);
	Triangle tree24(tree2X, tree2Y + 65, 50.0f, 50.0f);
	tree24.SetShader(&noTextureShader);
	tree22.SetColor(0.156f, 0.3f, 0.08f);
	tree23.SetColor(0.156f, 0.3f, 0.08f);
	tree24.SetColor(0.156f, 0.3f, 0.08f);

	float tree3X = 660;
	float tree3Y = -540;
	Rectangle tree31(tree3X, tree3Y, 20.0f, 50.0f);
	tree31.SetTexture(treeTexture);
	Triangle tree32(tree3X, tree3Y + 25, 60.0f, 50.0f);
	tree32.SetShader(&noTextureShader);
	Triangle tree33(tree3X, tree3Y + 45, 55.0f, 50.0f);
	tree33.SetShader(&noTextureShader);
	Triangle tree34(tree3X, tree3Y + 65, 50.0f, 50.0f);
	tree34.SetShader(&noTextureShader);
	tree32.SetColor(0.156f, 0.3f, 0.08f);
	tree33.SetColor(0.156f, 0.3f, 0.08f);
	tree34.SetColor(0.156f, 0.3f, 0.08f);

	float tree4X = 670;
	float tree4Y = -260;
	Rectangle tree41(tree4X, tree4Y, 20.0f, 50.0f);
	tree41.SetTexture(treeTexture);
	Triangle tree42(tree4X, tree4Y + 25, 60.0f, 50.0f);
	tree42.SetShader(&noTextureShader);
	Triangle tree43(tree4X, tree4Y + 45, 55.0f, 50.0f);
	tree43.SetShader(&noTextureShader);
	Triangle tree44(tree4X, tree4Y + 65, 50.0f, 50.0f);
	tree44.SetShader(&noTextureShader);
	tree42.SetColor(0.156f, 0.3f, 0.08f);
	tree43.SetColor(0.156f, 0.3f, 0.08f);
	tree44.SetColor(0.156f, 0.3f, 0.08f);

	float tree5X = 660;
	float tree5Y = 0;
	Rectangle tree51(tree5X, tree5Y, 20.0f, 50.0f);
	tree51.SetTexture(treeTexture);
	Triangle tree52(tree5X, tree5Y + 25, 60.0f, 50.0f);
	tree52.SetShader(&noTextureShader);
	Triangle tree53(tree5X, tree5Y + 45, 55.0f, 50.0f);
	tree53.SetShader(&noTextureShader);
	Triangle tree54(tree5X, tree5Y + 65, 50.0f, 50.0f);
	tree54.SetShader(&noTextureShader);
	tree52.SetColor(0.156f, 0.3f, 0.08f);
	tree53.SetColor(0.156f, 0.3f, 0.08f);
	tree54.SetColor(0.156f, 0.3f, 0.08f);

	float tree6X = 610;
	float tree6Y = 30;
	Rectangle tree61(tree6X, tree6Y, 20.0f, 50.0f);
	tree61.SetTexture(treeTexture);
	Triangle tree62(tree6X, tree6Y + 25, 60.0f, 50.0f);
	tree62.SetShader(&noTextureShader);
	Triangle tree63(tree6X, tree6Y + 45, 55.0f, 50.0f);
	tree63.SetShader(&noTextureShader);
	Triangle tree64(tree6X, tree6Y + 65, 50.0f, 50.0f);
	tree64.SetShader(&noTextureShader);
	tree62.SetColor(0.156f, 0.3f, 0.08f);
	tree63.SetColor(0.156f, 0.3f, 0.08f);
	tree64.SetColor(0.156f, 0.3f, 0.08f);

	float tree7X = 610;
	float tree7Y = 200;
	Rectangle tree71(tree7X, tree7Y, 20.0f, 50.0f);
	tree71.SetTexture(treeTexture);
	Triangle tree72(tree7X, tree7Y + 25, 60.0f, 50.0f);
	tree72.SetShader(&noTextureShader);
	Triangle tree73(tree7X, tree7Y + 45, 55.0f, 50.0f);
	tree73.SetShader(&noTextureShader);
	Triangle tree74(tree7X, tree7Y + 65, 50.0f, 50.0f);
	tree74.SetShader(&noTextureShader);
	tree72.SetColor(0.156f, 0.3f, 0.08f);
	tree73.SetColor(0.156f, 0.3f, 0.08f);
	tree74.SetColor(0.156f, 0.3f, 0.08f);

	float tree8X = 560;
	float tree8Y = 230;
	Rectangle tree81(tree8X, tree8Y, 20.0f, 50.0f);
	tree81.SetTexture(treeTexture);
	Triangle tree82(tree8X, tree8Y + 25, 60.0f, 50.0f);
	tree82.SetShader(&noTextureShader);
	Triangle tree83(tree8X, tree8Y + 45, 55.0f, 50.0f);
	tree83.SetShader(&noTextureShader);
	Triangle tree84(tree8X, tree8Y + 65, 50.0f, 50.0f);
	tree84.SetShader(&noTextureShader);
	tree82.SetColor(0.156f, 0.3f, 0.08f);
	tree83.SetColor(0.156f, 0.3f, 0.08f);
	tree84.SetColor(0.156f, 0.3f, 0.08f);

	float tree9X = 410;
	float tree9Y = 260;
	Rectangle tree91(tree9X, tree9Y, 20.0f, 50.0f);
	tree91.SetTexture(treeTexture);
	Triangle tree92(tree9X, tree9Y + 25, 60.0f, 50.0f);
	tree92.SetShader(&noTextureShader);
	Triangle tree93(tree9X, tree9Y + 45, 55.0f, 50.0f);
	tree93.SetShader(&noTextureShader);
	Triangle tree94(tree9X, tree9Y + 65, 50.0f, 50.0f);
	tree94.SetShader(&noTextureShader);
	tree92.SetColor(0.156f, 0.3f, 0.08f);
	tree93.SetColor(0.156f, 0.3f, 0.08f);
	tree94.SetColor(0.156f, 0.3f, 0.08f);

	float tree10X = 260;
	float tree10Y = 240;
	Rectangle tree101(tree10X, tree10Y, 20.0f, 50.0f);
	tree101.SetTexture(treeTexture);
	Triangle tree102(tree10X, tree10Y + 25, 60.0f, 50.0f);
	tree102.SetShader(&noTextureShader);
	Triangle tree103(tree10X, tree10Y + 45, 55.0f, 50.0f);
	tree103.SetShader(&noTextureShader);
	Triangle tree104(tree10X, tree10Y + 65, 50.0f, 50.0f);
	tree104.SetShader(&noTextureShader);
	tree102.SetColor(0.156f, 0.3f, 0.08f);
	tree103.SetColor(0.156f, 0.3f, 0.08f);
	tree104.SetColor(0.156f, 0.3f, 0.08f);

	float tree11X = 440;
	float tree11Y = 100;
	Rectangle tree111(tree11X, tree11Y, 20.0f, 50.0f);
	tree111.SetTexture(treeTexture);
	Triangle tree112(tree11X, tree11Y + 25, 60.0f, 50.0f);
	tree112.SetShader(&noTextureShader);
	Triangle tree113(tree11X, tree11Y + 45, 55.0f, 50.0f);
	tree113.SetShader(&noTextureShader);
	Triangle tree114(tree11X, tree11Y + 65, 50.0f, 50.0f);
	tree114.SetShader(&noTextureShader);
	tree112.SetColor(0.156f, 0.3f, 0.08f);
	tree113.SetColor(0.156f, 0.3f, 0.08f);
	tree114.SetColor(0.156f, 0.3f, 0.08f);

	float tree12X = 310;
	float tree12Y = 110;
	Rectangle tree121(tree12X, tree12Y, 20.0f, 50.0f);
	tree121.SetTexture(treeTexture);
	Triangle tree122(tree12X, tree12Y + 25, 60.0f, 50.0f);
	tree122.SetShader(&noTextureShader);
	Triangle tree123(tree12X, tree12Y + 45, 55.0f, 50.0f);
	tree123.SetShader(&noTextureShader);
	Triangle tree124(tree12X, tree12Y + 65, 50.0f, 50.0f);
	tree124.SetShader(&noTextureShader);
	tree122.SetColor(0.156f, 0.3f, 0.08f);
	tree123.SetColor(0.156f, 0.3f, 0.08f);
	tree124.SetColor(0.156f, 0.3f, 0.08f);

	float tree13X = 240;
	float tree13Y = 30;
	Rectangle tree131(tree13X, tree13Y, 20.0f, 50.0f);
	tree131.SetTexture(treeTexture);
	Triangle tree132(tree13X, tree13Y + 25, 60.0f, 50.0f);
	tree132.SetShader(&noTextureShader);
	Triangle tree133(tree13X, tree13Y + 45, 55.0f, 50.0f);
	tree133.SetShader(&noTextureShader);
	Triangle tree134(tree13X, tree13Y + 65, 50.0f, 50.0f);
	tree134.SetShader(&noTextureShader);
	tree132.SetColor(0.156f, 0.3f, 0.08f);
	tree133.SetColor(0.156f, 0.3f, 0.08f);
	tree134.SetColor(0.156f, 0.3f, 0.08f);

	float tree14X = 530;
	float tree14Y = -20;
	Rectangle tree141(tree14X, tree14Y, 20.0f, 50.0f);
	tree141.SetTexture(treeTexture);
	Triangle tree142(tree14X, tree14Y + 25, 60.0f, 50.0f);
	tree142.SetShader(&noTextureShader);
	Triangle tree143(tree14X, tree14Y + 45, 55.0f, 50.0f);
	tree143.SetShader(&noTextureShader);
	Triangle tree144(tree14X, tree14Y + 65, 50.0f, 50.0f);
	tree144.SetShader(&noTextureShader);
	tree142.SetColor(0.156f, 0.3f, 0.08f);
	tree143.SetColor(0.156f, 0.3f, 0.08f);
	tree144.SetColor(0.156f, 0.3f, 0.08f);

	float tree15X = 220;
	float tree15Y = -220;
	Rectangle tree151(tree15X, tree15Y, 20.0f, 50.0f);
	tree151.SetTexture(treeTexture);
	Triangle tree152(tree15X, tree15Y + 25, 60.0f, 50.0f);
	tree152.SetShader(&noTextureShader);
	Triangle tree153(tree15X, tree15Y + 45, 55.0f, 50.0f);
	tree153.SetShader(&noTextureShader);
	Triangle tree154(tree15X, tree15Y + 65, 50.0f, 50.0f);
	tree154.SetShader(&noTextureShader);
	tree152.SetColor(0.156f, 0.3f, 0.08f);
	tree153.SetColor(0.156f, 0.3f, 0.08f);
	tree154.SetColor(0.156f, 0.3f, 0.08f);

	float tree16X = 320;
	float tree16Y = -350;
	Rectangle tree161(tree16X, tree16Y, 20.0f, 50.0f);
	tree161.SetTexture(treeTexture);
	Triangle tree162(tree16X, tree16Y + 25, 60.0f, 50.0f);
	tree162.SetShader(&noTextureShader);
	Triangle tree163(tree16X, tree16Y + 45, 55.0f, 50.0f);
	tree163.SetShader(&noTextureShader);
	Triangle tree164(tree16X, tree16Y + 65, 50.0f, 50.0f);
	tree164.SetShader(&noTextureShader);
	tree162.SetColor(0.156f, 0.3f, 0.08f);
	tree163.SetColor(0.156f, 0.3f, 0.08f);
	tree164.SetColor(0.156f, 0.3f, 0.08f);

	//LAKE
	float lakeX = 560;
	float lakeY = -304;

	Rectangle lake1(lakeX, lakeY, 96, 96);
	lake1.SetTexture(waterTexture);
	Rectangle lake2(lakeX - 64, lakeY - 32, 96, 96);
	lake2.SetTexture(waterTexture);
	Rectangle lake3(lakeX + 32, lakeY - 32, 96, 96);
	lake3.SetTexture(waterTexture);
	Rectangle lake4(lakeX - 96, lakeY - 64, 96, 96);
	lake4.SetTexture(waterTexture);
	Rectangle lake5(lakeX + 96, lakeY - 64, 96, 96);
	lake5.SetTexture(waterTexture);
	Rectangle lake6(lakeX - 160, lakeY - 160, 96, 96);
	lake6.SetTexture(waterTexture);
	Rectangle lake7(lakeX - 64, lakeY - 160, 96, 96);
	lake7.SetTexture(waterTexture);
	Rectangle lake8(lakeX + 64, lakeY - 160, 96, 96);
	lake8.SetTexture(waterTexture);
	Rectangle lake9(lakeX - 128, lakeY - 224, 96, 96);
	lake9.SetTexture(waterTexture);
	Rectangle lake10(lakeX - 64, lakeY - 256, 96, 96);
	lake10.SetTexture(waterTexture);
	Rectangle lake11(lakeX + 32, lakeY - 224, 96, 96);
	lake11.SetTexture(waterTexture);
	Rectangle lake12(lakeX, lakeY - 128, 96, 96);
	lake12.SetTexture(waterTexture);

	//LIGHTING
	Shader lightSourceShader("Custom_Shaders/LightSourceVertexShader.txt", "Custom_Shaders/LightSourceFragmentShader.txt");
	Shader lightingShader("Custom_Shaders/lightingVertexShader.txt", "Custom_Shaders/lightingFragmentShader.txt");
	lightingShader.use();
	lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("lightColor", 1.0f, 0.7f, 0.11f);
	lightingShader.setVec2("lightPos", 20, 20);
	Rectangle lightSource(0, 0, 100, 100);
	lightSource.SetShader(&lightSourceShader);

	Rectangle lightingTest(100, 0, 70, 70);
	lightingTest.SetShader(&lightingShader);

	glm::vec2 lightPos = glm::vec2(20, 20);
	glm::vec3 lightColor = glm::vec3(0.999f, 0.999f, 0.999f);

	//TileMap (Floor)
	TileMap tileMap(46, 40);

	for (int i = 0; i < tileMap.GetSize(); i++)
	{
		tileMap.Tiles[i] = ((unsigned char)rand() % 2) + 2;
	}
	tileMap.TileSetHandle = testTexture;
	createRoad(&tileMap);
	//createFarmField(&tileMap, 17, 34, 8, 5);
	createFarmField(&tileMap, 33, 33, 11, 6);
	TileMapRenderer tileMapRenderer(tileMap);
	tileMapRenderer.shader->use();
	tileMapRenderer.shader->setVec2("lightPos", lightPos.x, lightPos.y);
	tileMapRenderer.shader->setVec3("lightColor", lightColor.x, lightColor.y, lightColor.z);

	gameObjects.push_back(player);
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
	gameObjects.push_back(&church1);
	gameObjects.push_back(&church9);
	gameObjects.push_back(&church2);
	gameObjects.push_back(&church3);
	gameObjects.push_back(&church4);
	gameObjects.push_back(&church5);
	gameObjects.push_back(&church6);
	gameObjects.push_back(&church7);
	gameObjects.push_back(&church8);
	gameObjects.push_back(&house17);
	gameObjects.push_back(&house11);
	gameObjects.push_back(&house12);
	gameObjects.push_back(&house13);
	gameObjects.push_back(&house14);
	gameObjects.push_back(&house15);
	gameObjects.push_back(&house16);
	gameObjects.push_back(&house18);
	gameObjects.push_back(&house19);
	gameObjects.push_back(&house110);
	gameObjects.push_back(&house111);
	gameObjects.push_back(&house112);
	gameObjects.push_back(&house113);
	gameObjects.push_back(&house114);
	gameObjects.push_back(&house27);
	gameObjects.push_back(&house21);
	gameObjects.push_back(&house22);
	gameObjects.push_back(&house23);
	gameObjects.push_back(&house24);
	gameObjects.push_back(&house25);
	gameObjects.push_back(&house26);
	gameObjects.push_back(&house28);
	gameObjects.push_back(&house29);
	gameObjects.push_back(&house210);
	gameObjects.push_back(&house211);
	gameObjects.push_back(&house212);
	gameObjects.push_back(&house213);
	gameObjects.push_back(&house214);
	gameObjects.push_back(&house37);
	gameObjects.push_back(&house31);
	gameObjects.push_back(&house32);
	gameObjects.push_back(&house33);
	gameObjects.push_back(&house34);
	gameObjects.push_back(&house35);
	gameObjects.push_back(&house36);
	gameObjects.push_back(&house38);
	gameObjects.push_back(&house39);
	gameObjects.push_back(&house310);
	gameObjects.push_back(&house311);
	gameObjects.push_back(&house312);
	gameObjects.push_back(&house313);
	gameObjects.push_back(&house314);
	gameObjects.push_back(&house47);
	gameObjects.push_back(&house41);
	gameObjects.push_back(&house42);
	gameObjects.push_back(&house43);
	gameObjects.push_back(&house44);
	gameObjects.push_back(&house45);
	gameObjects.push_back(&house46);
	gameObjects.push_back(&house48);
	gameObjects.push_back(&house49);
	gameObjects.push_back(&house410);
	gameObjects.push_back(&house411);
	gameObjects.push_back(&house412);
	gameObjects.push_back(&house413);
	gameObjects.push_back(&house414);
	gameObjects.push_back(&farmHouse7);
	gameObjects.push_back(&farmHouse1);
	gameObjects.push_back(&farmHouse2);
	gameObjects.push_back(&farmHouse3);
	gameObjects.push_back(&farmHouse4);
	gameObjects.push_back(&farmHouse5);
	gameObjects.push_back(&farmHouse6);
	gameObjects.push_back(&farmHouse8);
	gameObjects.push_back(&farmHouse9);
	gameObjects.push_back(&farmHouse10);
	gameObjects.push_back(&farmHouse11);
	gameObjects.push_back(&farmHouse12);
	gameObjects.push_back(&farmHouse13);
	gameObjects.push_back(&farmHouse14);
	gameObjects.push_back(&lake1);
	gameObjects.push_back(&lake2);
	gameObjects.push_back(&lake3);
	gameObjects.push_back(&lake4);
	gameObjects.push_back(&lake5);
	gameObjects.push_back(&lake6);
	gameObjects.push_back(&lake7);
	gameObjects.push_back(&lake8);
	gameObjects.push_back(&lake9);
	gameObjects.push_back(&lake10);
	gameObjects.push_back(&lake11);
	gameObjects.push_back(&lake12);
	gameObjects.push_back(&tree11);
	gameObjects.push_back(&tree12);
	gameObjects.push_back(&tree13);
	gameObjects.push_back(&tree14);
	gameObjects.push_back(&tree21);
	gameObjects.push_back(&tree22);
	gameObjects.push_back(&tree23);
	gameObjects.push_back(&tree24);
	gameObjects.push_back(&tree31);
	gameObjects.push_back(&tree32);
	gameObjects.push_back(&tree33);
	gameObjects.push_back(&tree34);
	gameObjects.push_back(&tree41);
	gameObjects.push_back(&tree42);
	gameObjects.push_back(&tree43);
	gameObjects.push_back(&tree44);
	gameObjects.push_back(&tree51);
	gameObjects.push_back(&tree52);
	gameObjects.push_back(&tree53);
	gameObjects.push_back(&tree54);
	gameObjects.push_back(&tree61);
	gameObjects.push_back(&tree62);
	gameObjects.push_back(&tree63);
	gameObjects.push_back(&tree64);
	gameObjects.push_back(&tree71);
	gameObjects.push_back(&tree72);
	gameObjects.push_back(&tree73);
	gameObjects.push_back(&tree74);
	gameObjects.push_back(&tree81);
	gameObjects.push_back(&tree82);
	gameObjects.push_back(&tree83);
	gameObjects.push_back(&tree84);
	gameObjects.push_back(&tree91);
	gameObjects.push_back(&tree92);
	gameObjects.push_back(&tree93);
	gameObjects.push_back(&tree94);
	gameObjects.push_back(&tree101);
	gameObjects.push_back(&tree102);
	gameObjects.push_back(&tree103);
	gameObjects.push_back(&tree104);
	gameObjects.push_back(&tree111);
	gameObjects.push_back(&tree112);
	gameObjects.push_back(&tree113);
	gameObjects.push_back(&tree114);
	gameObjects.push_back(&tree121);
	gameObjects.push_back(&tree122);
	gameObjects.push_back(&tree123);
	gameObjects.push_back(&tree124);
	gameObjects.push_back(&tree131);
	gameObjects.push_back(&tree132);
	gameObjects.push_back(&tree133);
	gameObjects.push_back(&tree134);
	gameObjects.push_back(&tree141);
	gameObjects.push_back(&tree142);
	gameObjects.push_back(&tree143);
	gameObjects.push_back(&tree144);
	gameObjects.push_back(&tree151);
	gameObjects.push_back(&tree152);
	gameObjects.push_back(&tree153);
	gameObjects.push_back(&tree154);
	gameObjects.push_back(&tree161);
	gameObjects.push_back(&tree162);
	gameObjects.push_back(&tree163);
	gameObjects.push_back(&tree164);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera2D* camera = player->GetCamera();

    //rendering loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        StaticGameInfo::GameTime += deltaTime;
        lastFrame = currentFrame;

		processInput(window, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        for_each(gameObjects.begin(), gameObjects.end(), [](GameObject* obj) {
            obj->Update(deltaTime);
        });

		glm::mat4 projectionMatrix = camera->GetProjectionMatrix(viewportWidth, viewportHeight);

		glm::mat4 tileMapProjectionMatrix = tileMapRenderer.GetProjectionMatrix(viewportWidth, viewportHeight, camera->X, camera->Y);
		tileMapRenderer.Render(tileMapProjectionMatrix);

		glm::vec2 lightPos = tileMapRenderer.GetTileMapCoordinates(player->X, player->Y, viewportWidth, viewportHeight);
		tileMapRenderer.shader->setVec2("lightPos", lightPos.x, lightPos.y);

		for_each(gameObjects.begin(), gameObjects.end(), [projectionMatrix](GameObject* obj) {
			obj->Render(projectionMatrix);
		});

		glm::mat4 textProjectionMatrix = glm::ortho(0.0f, static_cast<float>(viewportWidth), 0.0f, static_cast<float>(viewportHeight));
		textRenderer.RenderText("Coins: 0/1", 620.0f, 550.0f, 0.7f, glm::vec3(1, 1.0f, 1.0f), textProjectionMatrix);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete(player);

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
	player->NotifyInput(' ');

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		player->NotifyInput('w');
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		player->NotifyInput('s');
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		player->NotifyInput('a');
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		player->NotifyInput('d');
	}
}

GLenum errorCheck()
{
	GLenum code;
	const GLubyte* string;
	code = glGetError();

	return code;
}

void createFarmField(TileMap* tileMap, int x, int y, int width, int height) 
{
	for (int i = 0; i < width; ++i) 
	{
		for (int j = 0; j < height; ++j) 
		{
			if(i % 2 == 0)
				tileMap->Tiles[(x + i) + (y + j) * tileMap->Width] = 1;
			else
				tileMap->Tiles[(x + i) + (y + j) * tileMap->Width] = 0;
		}
	}
}

void createRoad(TileMap* tileMap)
{
	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			tileMap->Tiles[(3 + i) + (31 - j) * tileMap->Width] = 0;
		}
	}
	createRoad3x3(tileMap, 14, 28);
	createRoad3x3(tileMap, 16, 27);
	createRoad3x3(tileMap, 19, 27);
	createRoad3x3(tileMap, 22, 27);
	createRoad3x3(tileMap, 24, 29);
	createRoad3x3(tileMap, 26, 31);
	createRoad3x3(tileMap, 28, 32);
	//createRoad3x3(tileMap, 22, 24);
	createRoad3x3(tileMap, 20, 24);
	//createRoad3x3(tileMap, 23, 21);
	createRoad3x3(tileMap, 21, 21);
	createRoad3x3(tileMap, 22, 18);
	createRoad3x3(tileMap, 23, 18);
	createRoad3x3(tileMap, 22, 15);
	createRoad3x3(tileMap, 22, 12);
	createRoad3x3(tileMap, 21, 9);
	createRoad3x3(tileMap, 20, 8);
	createRoad3x3(tileMap, 18, 7);
	createRoad3x3(tileMap, 15, 7);
	createRoad3x3(tileMap, 26, 18);
	createRoad3x3(tileMap, 29, 17);
	createRoad3x3(tileMap, 32, 17);
	createRoad3x3(tileMap, 34, 16);
	createRoad3x3(tileMap, 36, 15);
	createRoad3x3(tileMap, 39, 14);
}

void createRoad3x3(TileMap* tileMap, int centerX, int centerY) 
{
	tileMap->Tiles[(centerX) + (centerY) * tileMap->Width] = 0;
	tileMap->Tiles[(centerX - 1)+(centerY)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX + 1)+(centerY)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX - 1)+(centerY - 1)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX)+(centerY - 1)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX + 1)+(centerY + 1)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX)+(centerY + 1)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX + 1)+(centerY - 1)*tileMap->Width] = 0;
	tileMap->Tiles[(centerX - 1)+(centerY + 1)*tileMap->Width] = 0;

}

/*void createHouse(vector<GameObject>* gameObjects, int x, int y, Shader* noTextureShader, unsigned int doorTexture, unsigned int chimneyTexture, float r, float g, float b)
{
	Rectangle house11(-200, -700, 120.0f, 100.0f);
	house11.SetShader(noTextureShader);
	Triangle house12(-200, -620, 120.0f, 60.0f);
	house12.SetShader(noTextureShader);
	Rectangle house13(-230, -720, 30.0f, 60.0f);
	house13.SetTexture(doorTexture);
	Rectangle house14(-170, -690, 30.0f, 30.0f);
	house14.SetShader(noTextureShader);
	Rectangle house15(-233, -625, 100.0f, 5.0f);
	house15.SetRotationZAxis(45);
	house15.SetTexture(texture);
	Rectangle house16(-165, -625, 100.0f, 5.0f);
	house16.SetRotationZAxis(135);
	house16.SetTexture(texture);
	Rectangle house17(-240, -620, 25.0f, 45.0f);
	house17.SetTexture(chimneyTexture);
	Rectangle house18(-240, -597.5f, 35, 5);
	house18.SetTexture(chimneyTexture);
	Rectangle house19(-170, -675, 32, 3);
	house19.SetTexture(texture);
	Rectangle house110(-170, -705, 38, 5);
	house110.SetTexture(texture);
	Rectangle house111(-185, -690, 32, 3);
	house111.SetRotationZAxis(90);
	house111.SetTexture(texture);
	Rectangle house112(-155, -690, 32, 3);
	house112.SetRotationZAxis(90);
	house112.SetTexture(texture);
	Rectangle house113(-170, -690, 32, 3);
	house113.SetRotationZAxis(90);
	house113.SetTexture(texture);
	Rectangle house114(-170, -690, 32, 3);
	house114.SetTexture(texture);
}
*/