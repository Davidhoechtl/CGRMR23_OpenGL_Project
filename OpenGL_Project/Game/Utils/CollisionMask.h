
#include "../Game/Objects/Collider.h"
#include "../Game/Objects/IHasCollision.h"

#pragma once
/// <summary>
/// Manages the collision between objects
/// </summary>
class CollisionMask
{
	public:
		IHasCollision* MoveAndCollide(IHasCollision* object, float deltaX, float deltaY) {
			
			if(deltaX == 0 && deltaY == 0)
				return nullptr;

			Collider* collider = object->GetCollider();
			collider->X += deltaX;
			collider->Y += deltaY;

			for (IHasCollision* nextCollider : colliders) {
				if (nextCollider->GetCollider()->CollidesWith(*collider)) {
					collider->X -= deltaX;
					collider->Y -= deltaY;
					return nextCollider;
				}
			}

			collider->X -= deltaX;
			collider->Y -= deltaY;
			return nullptr;
		}

		void AddCollider(IHasCollision* collider) {
			colliders.push_back(collider);
		}

		void RemoveCollider(IHasCollision* collider) {
			for (int i = 0; i < colliders.size(); i++) {
				if (colliders[i] == collider) {
					colliders.erase(colliders.begin() + i);
					return;
				}
			}
		}

		~CollisionMask() {
			for(IHasCollision* collider : colliders)
				delete collider;
		}

	private:
		vector<IHasCollision*> colliders;
};

