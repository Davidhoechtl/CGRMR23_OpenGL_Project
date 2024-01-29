#include "Player.h"
#include "../ImageLoader/ImageLoader.h"
#include "../Coin.h"

Player::Player(float x, float y) : Rectangle(x, y, 150, 150)
{
	camera = new Camera2D(x, y, 1.0f);
	InitAnimationPlayer();
	SetupCollider(x, y, 50, 75); // sets up collider
}

void Player::Update(float deltaTime) {

	// Move the player
	direction.normalize();
	float velocityX = direction.x * speed * deltaTime;
	float velocityY = direction.y * speed * deltaTime;

	IHasCollision* collision = collisionMask->MoveAndCollide(dynamic_cast<IHasCollision*>(this), velocityX, velocityY);
	Coin* coin = dynamic_cast<Coin*>(collision);
	if (coin != nullptr) {
		coin->toBeDestroyed = true;
		collisionMask->RemoveCollider(coin);
	}

	if (collision == nullptr) {
		X += velocityX;
		Y += velocityY;

		// Stop player at border
		int borderTop = 650;
		int borderBot = -610;
		int borderRight = 720;
		int borderLeft = -720;

		if (X >= borderRight)
			X = borderRight;

		if (X <= borderLeft)
			X = borderLeft;

		if (Y <= borderBot)
			Y = borderBot;

		if (Y >= borderTop)
			Y = borderTop;

		// Update the camera
		int cameraWidth = 380;
		int cameraHeight = 270;
		if (X <= borderRight - cameraWidth && X >= borderLeft + cameraWidth)
			camera->X = X;

		if (Y <= borderTop - cameraHeight + 40 && Y >= borderBot + cameraHeight)
			camera->Y = Y;

		// Update the collider
		collider->X = X;
		collider->Y = Y;
	}

	// update the texture
	texture = animationPlayer.GetNextTextureId();

	Rectangle::Update(deltaTime);
	collider->Update(deltaTime);
}

void Player::NotifyInput(char key) {
	if (key == 'w') {
		direction.y = 1;
		animationPlayer.SetCurrentTrackByName("RunUp");
	}
	else if (key == 's') {
		direction.y = -1;
		animationPlayer.SetCurrentTrackByName("RunDown");
	}
	else if (key == 'a') {
		direction.x = -1;
		animationPlayer.SetCurrentTrackByName("RunLeft");
	}
	else if (key == 'd') {
		direction.x = 1;
		animationPlayer.SetCurrentTrackByName("RunLeft");
	}
	else if (key == ' ') {
		direction.x = 0;
		direction.y = 0;

		animationPlayer.SetCurrentTrackByName("Idle");
	}
}

bool Player::CheckCollision(const Collider& other) {
	if (collider == nullptr)
		return false;

	return collider->CollidesWith(other);
}

Camera2D* Player::GetCamera() const {
	return camera;
}

void Player::InitAnimationPlayer() {
	ImageLoader imageLoader;

	string idlePath = resourcePath + "Idle/";
	AnimationTrack* idleTrack = new AnimationTrack(imageLoader, idlePath, "Idle", 100);

	string runLeftPath = resourcePath + "RunLeft/";
	AnimationTrack* runLeftTrack = new AnimationTrack(imageLoader, runLeftPath, "RunLeft", 100);

	string runUpPath = resourcePath + "RunUp/";
	AnimationTrack* runUpTrack = new AnimationTrack(imageLoader, runUpPath, "RunUp", 100);

	string runDownPath = resourcePath + "RunDown/";
	AnimationTrack* runDownTrack = new AnimationTrack(imageLoader, runDownPath, "RunDown", 100);

	animationPlayer.AddTrack(idleTrack);
	animationPlayer.AddTrack(runLeftTrack);
	animationPlayer.AddTrack(runUpTrack);
	animationPlayer.AddTrack(runDownTrack);

	animationPlayer.SetCurrentTrackByName("Idle");

	texture = animationPlayer.GetNextTextureId();
}

Player::~Player()
{
	animationPlayer.~AnimationPlayer();
	delete(camera);
}