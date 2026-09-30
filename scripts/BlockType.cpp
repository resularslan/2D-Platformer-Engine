#include "BlockType.h"
#include "CollisionManager.h"

BlockType::BlockType(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager)
	:
	Entity(xPos, yPos, width, height, renderer, camera, id),
	_collisionManager(collisionManager)
{}

void BlockType::init()
{
	thisCollisionMargin = { 8, 0 };
	moveTimer = 0;
	destroyTimer = 0;
	collisionMargin = thisCollisionMargin;
	Entity::init();
	type = EntityType::BlockType;
	originalY = rect.y;
}

void BlockType::restart()
{
	thisCollisionMargin = { 0, 0 };
	moveTimer = 0;
	destroyTimer = 0;
	collisionMargin = thisCollisionMargin;
	Entity::init();
	originalY = rect.y;
}

void BlockType::move()
{
	moveTimer = 0;
}