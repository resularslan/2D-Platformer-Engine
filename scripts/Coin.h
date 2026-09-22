#pragma once
#include "InteractableType.h"

class Player;

class Coin : public InteractableType
{
public:
	Coin(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, Player* player);
	~Coin();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void spawn(float xPos, float yPos) override;
protected:

private:
	SDL_Texture* texture;
	Player* _player;
	const float frameDelay = 0.025f;
	const float coinSpawnTime = 0.5f;
	const float coinSpawnSpeed = 250;
};
