#pragma once

#include "Entity.h"

enum class TurtleState
{
	Alive,
	Sleeping,
	Dying
};

class Player;

class Turtle : public Entity
{
public:
	Turtle(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id);
	~Turtle();
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
	void sleep();
	SDL_Texture* walkFrames[2];
	SDL_Texture* sleepingFrame;
	SDL_Texture* deathFrame;
	vector2 walkDirection = vector2::left;
	float oldWalkDirectionX = walkDirection.x;
	float speed;
	const float walkSpeed = 75;
	const float deathSpeed = 100;
	const float fastSpeed = 400;
	const float walkAnimationFrameDelay = 0.2f;
	const float deathJumpForce = 300;
	bool wasInCamera = true;
	float wakeTimer = 0;
	const float wakeTime = 5;
	TurtleState currentState = TurtleState::Alive;
};

