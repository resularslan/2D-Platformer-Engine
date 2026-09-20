#include "Star.h"
#include "Player.h"

Star::Star(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	InteractableType(xPos, yPos, width, height, renderer, camera, id),
	texture(nullptr)
{}

Star::~Star()
{
	SDL_DestroyTexture(texture);
}

void Star::init()
{
	InteractableType::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/Stars.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	gravity = starGravity;
	itemType = ItemType::Star;
}

void Star::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_FRect srcRect = { frameIndex * 17.0f, 0, 16, 16 };
	SDL_RenderTextureRotated(_renderer, texture, &srcRect, &newRect, 0, &center, flip);
}

void Star::onCollisionWithEntity(Entity* entity, Direction direction)
{
	switch (entity->getType())
	{
	case EntityType::PlayerType:
		dynamic_cast<Player*>(entity)->starMode();
		isActive = false;
		break;
	}
}

void Star::verticalMovement(float deltaTime)
{
	if (isGrounded)
	{
		velocity.y = -starJumpForce;
	}
}
