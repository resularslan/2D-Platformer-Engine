#pragma once

#include "Entity.h"

enum class InsectState
{
	Alive,
	Crushed,
	Reversed
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
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void die();
	SDL_Texture* walkFrames[2];
	SDL_Texture* crushedFrame;
	SDL_Texture* reversedFrame;
	vector2 walkDirection = vector2::left;
	const float walkSpeed = 100;
	const float walkAnimationFrameDelay = 0.4f;
	float destroyTimer = 0;
	const float destroyTime = 1;
	InsectState currentState = InsectState::Alive;
};