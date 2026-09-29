#include "Coin.h"
#include "Player.h"

Coin::Coin(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, Player* player)
	:
	InteractableType(xPos, yPos, width, height, renderer, camera, id),
	texture(nullptr),
	_player(player)
{}

Coin::~Coin()
{
	SDL_DestroyTexture(texture);
}

void Coin::init()
{
	InteractableType::init();
	texture = IMG_LoadTexture(_renderer, "assets/Items/Coins.png");
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	itemType = ItemType::Coin;
	canCollide = false;
}

void Coin::update(float deltaTime, CollisionManager& collisionManager)
{
	switch (currentState)
	{
	case InteractableTypeState::Spawning:
		animation(4, frameDelay, deltaTime);
		spawnTimer += deltaTime;
		if (spawnTimer < coinSpawnTime / 2)
		{
			rect.y -= coinUpSpeed * deltaTime;
		}
		else if (spawnTimer < coinSpawnTime)
		{
			
			rect.y += coinDownSpeed * deltaTime;
		}
		else
		{
			isActive = false;
		}
		break;
	default:
		break;
	}
}

void Coin::draw()
{
	SDL_FRect newRect = _camera.adjustToCamera(rect);
	SDL_FRect srcRect = { frameIndex * 17.0f, 0, 16, 16 };
	SDL_RenderTextureRotated(_renderer, texture, &srcRect, &newRect, 0, &center, flip);
}

void Coin::restart()
{
	InteractableType::init();
	canCollide = false;
}

void Coin::spawn(float xPos, float yPos)
{
	InteractableType::spawn(xPos, yPos);
	_player->addCoin();
}
