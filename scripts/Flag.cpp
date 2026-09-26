#include "Flag.h"
#include "Player.h"

Flag::Flag(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id)
	:
	InteractableType(xPos, yPos, width, height, renderer, camera, id),
	texture(nullptr)
{}

Flag::~Flag()
{
	SDL_DestroyTexture(texture);
}

void Flag::init()
{
	InteractableType::init();
	isActive = true;
	texture = IMG_LoadTexture(_renderer, "assets/TileMap/Flag.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	gravity = 0;
}

void Flag::update(float deltaTime, CollisionManager& collisionManager)
{
	Entity::update(deltaTime, collisionManager);
}

void Flag::flagAnimation()
{
	velocity.x = 0;
	velocity.y = flagDownSpeed;
}

void Flag::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, texture, NULL, &newRect, 0, &center, flip);
}

bool& Flag::getGroundedState()
{
	return isGrounded;
}

void Flag::horizontalMovement(float deltaTime)
{
	return;
}
