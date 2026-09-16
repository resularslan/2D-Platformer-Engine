#pragma once
#include "Entity.h"

enum class StarState
{
	Spawning,
	Alive
};

class Star : public Entity
{
public:
	Star(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	~Star();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void horizontalMovement(float deltaTime) override;
	void verticalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction, SDL_FRect tileRect) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	SDL_Texture* texture;
	vector2 moveDirection = vector2::left;
	StarState currentState = StarState::Spawning;
	float spawnTimer = 0;
	const float spawnTime = 0.65f;
	const float spawnSpeed = 50;
	const float speed = 100;
	const float jumpForce = 370;
	const float starGravity = 1000;
	const float frameDelay = 0.05f;
};
