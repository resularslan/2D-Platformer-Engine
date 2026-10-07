#pragma once

#include "Enemy.h"

enum class TurtleState
{
	Alive,
	Sleeping,
	Sliding,
	Dying
};

class Player;

class Turtle : public Enemy
{
public:
	Turtle(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	~Turtle();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
	void die() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void sleep();
	SDL_Texture* walkFrames[2];
	SDL_Texture* sleepingFrame[2];
	SDL_Texture* deathFrame;
	float oldWalkDirectionX = walkDirection.x;
	const float fastSpeed = 370;
	const float walkAnimationFrameDelay = 0.2f;
	bool wasInCamera = true;
	float wakeTimer = 0;
	const float wakeTime = 5;
	const float wakeAnimationTime = 2.5;
	const float wakingAnimationFrameDelay = 0.6f;
	TurtleState currentState = TurtleState::Alive;
};

