#pragma once

#include "Entity.h"

enum class TurtleState
{
	Alive,
	Sleeping,
	Reversed
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
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void sleep();
	void die();
	SDL_Texture* walkFrames[2];
	SDL_Texture* sleepingFrame;
	SDL_Texture* reversedFrame;
	vector2 walkDirection = vector2::left;
	float speed = 100;
	const float walkSpeed = 100;
	const float fastSpeed = 400;
	const float walkAnimationFrameDelay = 0.4f;
	float wakeTimer = 0;
	const float wakeTime = 5;
	TurtleState currentState = TurtleState::Alive;
};

