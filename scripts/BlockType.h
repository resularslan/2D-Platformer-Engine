#pragma once
#include "Entity.h"

class BlockType : public Entity
{
public:
	BlockType(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager);
	void init() override;
	void restart() override;
protected:
	virtual void move();
	CollisionManager& _collisionManager;
	vector2 blockCollisionMargin = { 0, 0 };
	float originalY;
	float moveTimer = 0;
	const float moveTime = 0.3f;
	const float moveSpeed = 100;
	float destroyTimer = 0;
	const float destroyTime = 0.1f;
};
