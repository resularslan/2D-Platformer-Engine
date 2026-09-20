#include "HealthMushroom.h"
#include "Player.h"

HealthMushroom::HealthMushroom(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	InteractableType(xPos, yPos, width, height, renderer, camera, id),
	texture(nullptr)
{}

HealthMushroom::~HealthMushroom()
{
	SDL_DestroyTexture(texture);
}

void HealthMushroom::init()
{
	InteractableType::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/HealthMushroom.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
}

void HealthMushroom::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, texture, NULL, &newRect, 0, &center, flip);
}

void HealthMushroom::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->addLife();
		isActive = false;
		break;
	}
}