#pragma once
#include "Entity.h"

enum class BrickState
{
	Static,
	Moving
};

class Brick : public Entity
{
public:
	Brick(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager);
	~Brick();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void move();
	CollisionManager& _collisionManager;
	BrickState currentState = BrickState::Static;
	SDL_Texture* brickTexture;
	vector2 brickCollisionMargin = { 0, 0 };
	float originalY;
	float moveTimer = 0;
	const float moveTime = 0.3f;
	const float moveSpeed = 100;
};
