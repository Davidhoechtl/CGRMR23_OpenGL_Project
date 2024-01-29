#include "Rectangle.h"
#include "IHasCollision.h"
#include "../../ImageLoader/ImageLoader.h"

#pragma once
/// <summary>
/// Object that can be collected by the player
/// </summary>
class Coin : public Rectangle, public IHasCollision
{
	public:
		Coin(float x, float y) : Rectangle(x,y,50,50) {
			SetupCoin();
			SetupCollider(x, y, 50, 50);
		}

		void Update(float deltaTime) override {
			// add movement

			Rectangle::Update(deltaTime);
			collider->Update(deltaTime);
		}

		void Render(const glm::mat4 cameraProjectionMatrix) override {
			Rectangle::Render(cameraProjectionMatrix);
			collider->Render(cameraProjectionMatrix);
		}

		bool CheckCollision(const Collider& other) override {
			return collider->CollidesWith(other);
		}

	private:

		void SetupCoin() {
			// load texture
			ImageLoader imageLoader;
			texture = imageLoader.loadTransparentImage("Ressources/coin.png", true);
		}

		const string resourcePath = "Ressources/coin.png";
};

