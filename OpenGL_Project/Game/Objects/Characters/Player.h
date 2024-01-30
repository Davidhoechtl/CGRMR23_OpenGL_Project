#include "../Rectangle.h"
#include "../../AnimationPlayer/AnimationPlayer.h"
#include "../../Camera.h"
#include "../../Utils/Vector2D.h"
#include "../../Utils/CollisionMask.h"

#include <string>

#pragma once
class Player : public Rectangle, public IHasCollision
{
	public:
		int speed = 100;
		int score = 0;
		
		Player(float x, float y);
		~Player();

	    void Update(float deltaTime) override;
		void Render(const glm::mat4 cameraProjectionMatrix) override {
			collider->Render(cameraProjectionMatrix);
			Rectangle::Render(cameraProjectionMatrix);
		}
		void NotifyInput(char key);

		bool CheckCollision(const Collider& other) override;

		Camera2D* GetCamera() const;

		void SetCollisionMask(CollisionMask* collisionMask) {
			this->collisionMask = collisionMask;
		}

	private:
		void InitAnimationPlayer();

		AnimationPlayer animationPlayer;
		Camera2D* camera;
		Vector2D direction;
		CollisionMask* collisionMask;
		const string resourcePath = "Ressources/Player/";
};

