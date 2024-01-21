#ifndef GAMEOBJECT_H
#define IMAGELOADER_H

#include "../../Custom_Shaders/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

class GameObject
{
	public:
		float X, Y;

		GameObject(float x, float y) {
			X = x;
			Y = y;

			// Default shader
			shader = new Shader("Custom_Shaders/testVertexShader.txt", "Custom_Shaders/testFragmentShader.txt");
		}

		void SetRotationZAxis(float angle) 
		{
			rotationAngle = angle;
		}

		// Overrides the default shader
		void SetShader(Shader* shader) {
			this->shader = shader;
		}

		// Sets a texture for the object
		void SetTexture(unsigned int texture) {
			this->texture = texture;
		}

		// Method that gets called at the start of each frame
		virtual void Update(float delta) = 0;

		// Method that gets called at the end of each frame
		virtual void Render(const glm::mat4 cameraProjectionMatrix) = 0;

	protected:
		glm::mat4 transform = glm::mat4(1.0f);
		float rotationAngle = 0;

		Shader* shader = nullptr;
		unsigned int texture = 0;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;
};
#endif
