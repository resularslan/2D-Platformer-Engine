#include "FinishDoor.h"
#include "Player.h"
#include "CollisionManager.h"

FinishDoor::FinishDoor(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager)
	:
	BlockType(xPos, yPos, width, height, renderer, camera, id, collisionManager)
{}

void FinishDoor::init()
{
	BlockType::init();
}

void FinishDoor::update(float deltaTime, CollisionManager& collisionManager)
{
	return;
}

void FinishDoor::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	Entity::lateUpdate(deltaTime, collisionManager);
}

void FinishDoor::draw()
{
	return;
}

void FinishDoor::restart()
{
	return;
}

void FinishDoor::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		SDL_Quit();
		break;
	default:
		break;
	}
}
