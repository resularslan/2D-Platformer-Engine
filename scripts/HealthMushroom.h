#pragma once
#include "Entity.h"

enum class HealthMushroomState
{
	Spawning,
	Alive
};

class HealthMushroom : public Entity
{
public:
	HealthMushroom(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id);
	~HealthMushroom();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
	void jump();
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction, SDL_FRect tileRect) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	HealthMushroomState currentState = HealthMushroomState::Spawning;
	float spawnTimer = 0;
	const float spawnTime = 0.65f;
	const float spawnSpeed = 50;
	SDL_Texture* texture;
	vector2 moveDirection = vector2::left;
	const float speed = 100;
	const float jumpForce = 200;
};
