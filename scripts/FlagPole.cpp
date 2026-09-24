#include "FlagPole.h"
#include "Player.h"
#include "CollisionManager.h"

FlagPole::FlagPole(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager)
	:
	BlockType(xPos, yPos, width, height, renderer, camera, id, collisionManager),
	texture(nullptr)
{}

FlagPole::~FlagPole()
{
	SDL_DestroyTexture(texture);
}

void FlagPole::init()
{
	BlockType::init();
	texture = IMG_LoadTexture(_renderer, "assets/TileMap/FlagPole.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

void FlagPole::update(float deltaTime, CollisionManager& collisionManager)
{
	return;
}

void FlagPole::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	Entity::lateUpdate(deltaTime, collisionManager);
}

void FlagPole::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, texture, NULL, &newRect, 0, &center, flip);
}

void FlagPole::restart()
{
	return;
}

void FlagPole::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->flagAnimation();
		break;
	default:
		break;
	}
}
