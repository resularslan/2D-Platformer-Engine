#pragma once

#include "Entity.h"

enum class InsectState
{
	Alive,
	Crushed,
	Dying
};

class Player;

class Insect : public Entity
{
public:
	Insect(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id);
	~Insect();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
	void die() override;
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction, SDL_FRect tileRect) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void crush();
	SDL_Texture* walkFrames[2];
	SDL_Texture* crushedFrame;
	SDL_Texture* deathFrame;
	vector2 walkDirection = vector2::left;
	const float walkSpeed = 75;
	const float walkAnimationFrameDelay = 0.2f;
	const float deathJumpForce = 300;
	float destroyTimer = 0;
	const float destroyTime = 1;
	InsectState currentState = InsectState::Alive;
};