
#include "Collider.h"

#pragma once
class IHasCollision
{
	public:

		virtual bool CheckCollision(const Collider& collider) = 0;

		Collider* GetCollider() const {
			return collider;
		}

		~IHasCollision() {
			delete collider;
		}

	protected:
		void SetupCollider(int x, int y, int width, int height) {
			collider = new Collider(x, y, width, height);
		}

		Collider* collider;
};

