#include "BrickPiece.h"

BrickPiece::BrickPiece(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id)
	:
	Entity(xPos, yPos, renderer, camera, id),
	textures(nullptr, nullptr)
{}

BrickPiece::~BrickPiece()
{
	SDL_DestroyTexture(textures[0]);
	SDL_DestroyTexture(textures[1]);
}

void BrickPiece::init()
{
	rect = { _xPos, _yPos, CELL_SIZE, CELL_SIZE };
	textures[0] = IMG_LoadTexture(_renderer, "assets/TileMap/BrickPiece-1.png");
	textures[1] = IMG_LoadTexture(_renderer, "assets/TileMap/BrickPiece-2.png");
	SDL_SetTextureScaleMode(textures[0], SDL_SCALEMODE_NEAREST);
	SDL_SetTextureScaleMode(textures[1], SDL_SCALEMODE_NEAREST);
	type = EntityType::InteractableType;
	canCollide = false;
	velocity.y = -jumpForce;
}

void BrickPiece::update(float deltaTime, CollisionManager& collisionManager)
{
	horizontalMovement(deltaTime);
	verticalMovement(deltaTime);
	if (velocity.y < maxVelocityY)
	{
		velocity.y += gravity * deltaTime;
		if (velocity.y > maxVelocityY)
		{
			velocity.y = maxVelocityY;
		}
	}
	rect.x += velocity.x * deltaTime;
	rect.y += velocity.y * deltaTime;
	animation(2, 0.5f, deltaTime);
}

void BrickPiece::lateUpdate(float deltaTime, CollisionManager& collisionManager)
{
	return;
}

void BrickPiece::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_RenderTextureRotated(_renderer, textures[frameIndex], NULL, &newRect, 0, &center, flip);
}

void BrickPiece::restart()
{
	return;
}

void BrickPiece::setDirectionX(float x)
{
	direction.x = x;
}

void BrickPiece::horizontalMovement(float deltaTime)
{
	velocity.x = direction.x * speed;
}
