#include "../Rectangle.h"
#include "../../AnimationPlayer/AnimationPlayer.h"
#include "../../Utils/Vector2D.h"
#include "../../Camera.h"

#include <string>

#pragma once
class Player : public Rectangle
{
	public:
		int speed = 200;
		int score = 0;
		
		Player(float x, float y);
		~Player();

	    void Update(float deltaTime) override;
		void NotifyInput(char key);

		Camera2D* GetCamera() const;

	private:
		void InitAnimationPlayer();

		AnimationPlayer animationPlayer;
		Camera2D* camera;
		Vector2D direction;
		const string resourcePath = "Ressources/Player/";
};

