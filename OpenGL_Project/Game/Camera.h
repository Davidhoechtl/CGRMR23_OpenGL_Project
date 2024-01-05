#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera2D
{
	public:
		float X, Y;
		float Zoom;
		float CameraSpeed;

		Camera2D(float x, float y, float zoom, float cameraSpeed) {
			X = x;
			Y = y;
			Zoom = zoom;
			CameraSpeed = cameraSpeed;
		}

		glm::mat4 GetProjectionMatrix(const float& viewportWidth, const float& viewportHeight) {
			float left = X - (viewportWidth / 2);
			float right = X + (viewportWidth / 2);
			float bottom = Y - (viewportHeight / 2);
		    float top = Y + (viewportHeight / 2);

			glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
			projection = glm::scale(projection, glm::vec3(Zoom, Zoom, 1.0f)); // apply zoom

			return projection;
		}
};