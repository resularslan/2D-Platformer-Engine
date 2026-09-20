#include "GrowMushroom.h"
#include "Player.h"

GrowMushroom::GrowMushroom(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	InteractableType(xPos, yPos, width, height, renderer, camera, id),
	texture(nullptr)
{}

GrowMushroom::~GrowMushroom()
{
	SDL_DestroyTexture(texture);
}

void GrowMushroom::init()
{
	InteractableType::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/GrowMushroom.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	itemType = ItemType::GrowMushroom;
}

void GrowMushroom::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, texture, NULL, &newRect, 0, &center, flip);
}

void GrowMushroom::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->grow();
		isActive = false;
		break;
	}
}
