#include <glad/glad.h>

class Triangle : public GameObject
{
	public:
		int Width;
		int Height;

		Triangle(float x, float y, float width, float height) : GameObject(x, y) {

			Width = width;
			Height = height;

			SetupTriangle();
		}

		~Triangle() {
			// cleanup gameobject
			//free(shader);
		}

		void Update(float delta) override {
			// here the model matrix can be manipulated
			transform = glm::mat4(1.0f);
			transform = glm::translate(transform, glm::vec3(X, Y, 0.0f));
			transform = glm::rotate(transform, glm::radians(rotationAngle), glm::vec3(0.0, 0.0, 1.0));
			transform = glm::scale(transform, glm::vec3(Width, Height, 1.0f));
		}

		void Render(const glm::mat4 cameraProjectionMatrix) override {
			if (shader == nullptr) {
				std::cout << "ERROR::TRIANGLE::SHADER_NOT_SET" << std::endl;
				return;
			}

			shader->use();
			
			unsigned int projectionLoc = glGetUniformLocation(shader->ID, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(cameraProjectionMatrix));

			unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}

		//sets vertices color
		void SetColor(float r, float g, float b)
		{
			vertices[3] = r;
			vertices[4] = g;
			vertices[5] = b;
			vertices[11] = r;
			vertices[12] = g;
			vertices[13] = b;
			vertices[19] = r;
			vertices[20] = g;
			vertices[21] = b;

			glDeleteVertexArrays(1, &VAO);
			VAO = 0;

			glDeleteBuffers(1, &VBO);
			VBO = 0;
			SetupTriangle();
		}

	private:
		// model vertices
		float vertices[24] = {
				-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,  // bottom right
				0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,   // bottom left
				0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.5f, 1.0f,  // top
		};

		void SetupTriangle() {

			// setup Vertex Buffer Object (VBO)
			//unsigned int VBO, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
		
};