#pragma once
#include "Entity.h"

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
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction, SDL_FRect tileRect) override;
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	SDL_Texture* texture;
	vector2 moveDirection = vector2::left;
	const float speed = 100;
};
