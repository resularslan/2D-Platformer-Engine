#pragma once

#include "Enemy.h"

enum class InsectState
{
	Alive,
	Crushed,
	Dying
};

class Player;

class Insect : public Enemy
{
public:
	Insect(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	~Insect();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
	void die() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void crush();
	SDL_Texture* walkFrames[2];
	SDL_Texture* crushedFrame;
	SDL_Texture* deathFrame;
	const float walkAnimationFrameDelay = 0.2f;
	float destroyTimer = 0;
	const float destroyTime = 1;
	InsectState currentState = InsectState::Alive;
};