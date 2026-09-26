#include "BlockType.h"
#include "CollisionManager.h"

BlockType::BlockType(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager)
	:
	Entity(xPos, yPos, width, height, renderer, camera, id),
	_collisionManager(collisionManager)
{}

void BlockType::init()
{
	collisionMargin = thisCollisionMargin;
	Entity::init();
	type = EntityType::BlockType;
	originalY = rect.y;
}

void BlockType::restart()
{
	return;
}

void BlockType::move()
{
	moveTimer = 0;
}