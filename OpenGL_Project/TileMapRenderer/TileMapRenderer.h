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

class TileMapRenderer 
{
public:
	glm::vec2 Center;
	TileMap _TileMap;
	const int TileSize = 32;
	const float TileTexSize = (float)1 / 16;
	const float TileTexPadding = (float)1 / 256;

	TileMapRenderer(TileMap tileMap)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClipControl(GL_UPPER_LEFT, GLM_DEPTH_NEGATIVE_ONE_TO_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Center = glm::vec2(0.0f, 0.0f);
        _TileMap = tileMap;
        CreateShader();
        GenerateVertexBufferObject();
        GenerateVertexArrayObject();
	}

    void Render(const glm::mat4 projectionMatrix)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, _TileMap.TileSetHandle);
        glBindVertexArray(VAO);
        
        unsigned int projectionLoc = glGetUniformLocation(shader->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        
        shader->use();
        glDrawArrays(GL_TRIANGLES, 0, _TileMap.GetSize() * 6);
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
	Shader* shader;
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

        const int floatCount = _TileMap.GetSize() // for each tile
            * 6 // there are 6 vertices (two triangles, each with 3 vertices)
            * 2 // each vertex has two components: Position and Texcoord
            * 2; // each component has two fields: x and y
        vector<float> vertexData;
        vertexData.resize(floatCount);
        //float* vertexData = new float[floatCount];
        //int i = 0;
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
                //i += 4;

                // vertex 1 (top right)
                vertexData.push_back(x + 1); // position x
                vertexData.push_back(y);// position y
                vertexData.push_back(tx0 + tySize); // texcoord x
                vertexData.push_back(ty0); // texcoord y
                //i += 4;

                // vertex 2 (bottom left)
                vertexData.push_back(x); // position x
                vertexData.push_back(y + 1);// position y
                vertexData.push_back(tx0); // texcoord x
                vertexData.push_back(ty0 + tySize); // texcoord y
                //i += 4;

                // vertex 3 (top right)
                vertexData.push_back(x + 1); // position x
                vertexData.push_back(y);// position y
                vertexData.push_back(tx0 + tySize); // texcoord x
                vertexData.push_back(ty0); // texcoord y
                //i += 4;

                // vertex 4 (bottom left)
                vertexData.push_back(x); // position x
                vertexData.push_back(y + 1);// position y
                vertexData.push_back(tx0); // texcoord x
                vertexData.push_back(ty0 + tySize); // texcoord y
                //i += 4;

                // vertex 5 (bottom right)
                vertexData.push_back(x + 1); // position x
                vertexData.push_back(y + 1);// position y
                vertexData.push_back(tx0 + tySize); // texcoord x
                vertexData.push_back(ty0 + tySize); // texcoord y
                //i += 4;
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