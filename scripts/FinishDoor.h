#pragma once
#include "BlockType.h"

class FinishDoor : public BlockType
{
public:
	FinishDoor(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager);
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
};
