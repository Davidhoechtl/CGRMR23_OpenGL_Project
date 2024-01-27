#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "TileMap.h"
#include <GLFW/glfw3.h>
#include "../Custom_Shaders/Shader.h"
#include <vector>

using namespace std;

class TileMapRenderer 
{
public:
	glm::vec2 Center;
	TileMap _TileMap;
	const int TileSize = 32;
	const float TileTexSize = (float)1 / 16;
	const float TileTexPadding = (float)1 / 256;
    Shader* shader;

	TileMapRenderer(TileMap tileMap)
	{
		//glClearColor(0.2f, 1.0f, 0.3f, 1.0f);
		//glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Center = glm::vec2(0.0f, 0.0f);
        _TileMap = tileMap;
        CreateShader();
        GenerateVertexBufferObject();             
        GenerateVertexArrayObject();
	}

    void Render(const glm::mat4 projectionMatrix)
    {
        if (shader == nullptr) {
            std::cout << "ERROR::TILEMAP::SHADER_NOT_SET" << std::endl;
            return;
        }
        shader->use();

        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int projectionLoc = glGetUniformLocation(shader->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(GetTransformMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _TileMap.TileSetHandle);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, _TileMap.GetSize() * 6);
        glBindVertexArray(0);
    }

    glm::mat4 GetProjectionMatrix(float viewportWidth, float viewportHeight, float cameraX, float cameraY) 
    {
        float left = cameraX - (viewportWidth / 2);
        float right = cameraX + (viewportWidth / 2);
        float bottom = cameraY - (viewportHeight / 2);
        float top = cameraY + (viewportHeight / 2);

        glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        projection = glm::scale(projection, glm::vec3(TileSize, TileSize, 1.0f));
        //projection = glm::scale(projection, glm::vec3(2.0f / backBufferWidth, 2.0f / backBufferHeight, 1.0f));

        return projection;
    }

    glm::mat4 GetTransformMatrix() 
    {
        //moves tileMap to center of screen
        float tileMapWidth = (float)_TileMap.Width;
        float tileMapHeight = (float)_TileMap.Height;
        
        glm::vec3 objectCenter = glm::vec3(tileMapWidth / 2, tileMapHeight / 2, 0.0f);

        glm::vec3 targetPoint = glm::vec3(-Center.x, -Center.y, 0);

        glm::vec3 translationVector = targetPoint - objectCenter;

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translationVector);
        
        return translationMatrix;
    }

    glm::vec2 GetTileMapCoordinates(float x, float y, int viewPortWidth, int viewPortHeight) 
    {
        float offsetX = -2;
        float offsetY = 0.7;
        glm::vec2 tileMapPos = glm::vec2((x + viewPortWidth) / (TileSize - TileTexPadding - TileTexSize) + offsetX, (y + viewPortHeight) / (TileSize - TileTexPadding - TileTexSize) + offsetY);

        return tileMapPos;
    }

    ~TileMapRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;

        glDeleteBuffers(1, &VBO);
        VBO = 0;

        //glDeleteProgram(shader.ID);
    }

    void OnBackBufferResized(int width, int height)
    {
        backBufferWidth = width;
        backBufferHeight = height;
        glViewport(0, 0, width, height);
    }

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
	int backBufferWidth, backBufferHeight;

    void CreateShader()
    {
		shader = new Shader("Custom_Shaders/tileMapVertexShader.txt", "Custom_Shaders/tileMapFragmentShader.txt");
    }

    void GenerateVertexBufferObject()
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        vector<float> vertexData;
        for (int x = 0; x < _TileMap.Width; x++)
        {
            for (int y = 0; y < _TileMap.Height; y++)
            {
                unsigned char tile = _TileMap.GetTile(x, y);
                float tx0 = (tile & 15) * TileTexSize + TileTexPadding;
                float ty0 = (tile >> 4) * TileTexSize + TileTexPadding;
                float tySize = TileTexSize - TileTexPadding * 2;

                // vertex 0 (top left)
                vertexData.push_back(x); // position x
                vertexData.push_back(y);// position y
                vertexData.push_back(tx0); // texcoord x
                vertexData.push_back(ty0); // texcoord y

                // vertex 1 (top right)
                vertexData.push_back(x + 1); // position x
                vertexData.push_back(y);// position y
                vertexData.push_back(tx0 + tySize); // texcoord x
                vertexData.push_back(ty0); // texcoord y

                // vertex 2 (bottom left)
                vertexData.push_back(x); // position x
                vertexData.push_back(y + 1);// position y
                vertexData.push_back(tx0); // texcoord x
                vertexData.push_back(ty0 + tySize); // texcoord y

                // vertex 3 (top right)
                vertexData.push_back(x + 1); // position x
                vertexData.push_back(y);// position y
                vertexData.push_back(tx0 + tySize); // texcoord x
                vertexData.push_back(ty0); // texcoord y

                // vertex 4 (bottom left)
                vertexData.push_back(x); // position x
                vertexData.push_back(y + 1);// position y
                vertexData.push_back(tx0); // texcoord x
                vertexData.push_back(ty0 + tySize); // texcoord y

                // vertex 5 (bottom right)
                vertexData.push_back(x + 1); // position x
                vertexData.push_back(y + 1);// position y
                vertexData.push_back(tx0 + tySize); // texcoord x
                vertexData.push_back(ty0 + tySize); // texcoord y
            }
        }
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
    }

    void GenerateVertexArrayObject()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

};