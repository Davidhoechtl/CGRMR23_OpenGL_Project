#pragma once

#include "../ImageLoader/ImageLoader.h"
#include "Rectangle.h"

/// <summary>
/// Colliders are recatangles that can collide with other colliders
/// </summary>
class Collider : public Rectangle
{
	public:
		bool render = true;

		Collider(int x, int y, int width, int height) : Rectangle(x,y,width,height){
			ImageLoader imageLoader;
			texture = imageLoader.loadTransparentImage(resourcePath, true);
		}

		void Update(float deltaTime) override {
			Rectangle::Update(deltaTime);
		}

		void Render(const glm::mat4 cameraProjectionMatrix) override {
			if(render)
				Rectangle::Render(cameraProjectionMatrix);
		}

		/// <summary>
		/// Checks if the collider collides with another collider (Origin of GameObject is the center!!!!!!)
		/// </summary>
		bool CollidesWith(const Collider& other) {
			// Calculate half-width and half-height for each rectangle
			double halfWidth1 = Width / 2;
			double halfHeight1 = Height / 2;
			double halfWidth2 = other.Width / 2;
			double halfHeight2 = other.Height / 2;

			// Calculate the distance between the centers of the rectangles along both axes
			double dx = std::abs(X - other.X);
			double dy = std::abs(Y - other.Y);

			// Check for overlap along both axes
			return (dx <= halfWidth1 + halfWidth2) && (dy <= halfHeight1 + halfHeight2);
		}

	private:
		const char* resourcePath = "Ressources/ColliderTexture.png";
};

