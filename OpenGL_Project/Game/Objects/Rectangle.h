#pragma once

#include <glad/glad.h>
#include "GameObject.h"

class Rectangle : public GameObject
{
	public:
		int Width;
		int Height;

		Rectangle(float x, float y, float width, float height) : GameObject(x, y){
			
			Width = width;
			Height = height;

			SetupRectangle();
		}

		~Rectangle() {
			// cleanup gameobject
			free(shader);
		}

		void Update() override {
			// here the model matrix can be manipulated
			transform = glm::mat4(1.0f);
			transform = glm::translate(transform, glm::vec3(X, Y, 0.0f));
		}

		void Render() override {
			if (shader == nullptr) {
				std::cout << "ERROR::RECTANGLE::SHADER_NOT_SET" << std::endl;
				return;
			}

			shader->use();

			unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	private:
		void SetupRectangle() {
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO); // bind Vertex Array Object

			// setup Vertex Buffer Object (VBO)
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

		// model vertices
		float vertices[32] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    2.0f, 2.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    2.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,    0.0f, 2.0f  // top left 
		};

		unsigned int indices[6] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
};

