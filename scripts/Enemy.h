#pragma once

#include "Entity.h"

class Player;

class Enemy : public Entity
{
public:
	Enemy(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	void init() override;
	void restart() override;
	virtual void die();
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction, SDL_FRect tileRect) override;
	vector2 walkDirection = vector2::left;
	float speed;
	const float walkSpeed = 75;
	const float deathSpeed = 100;
	float animationFrameDelay;
	const float deathJumpForce = 300;
};