#pragma once
#include "Entity.h"
#include <memory>

enum class BrickState
{
	Static,
	Moving,
	Breaking
};

class BrickPiece;

class Brick : public Entity
{
public:
	Brick(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, std::vector<BrickPiece*>& brickPieces);
	~Brick();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	void createBrickPiece(float directionX, float posX, float posY, float force);
	void move();
	void breaking();
	CollisionManager& _collisionManager;
	std::vector<BrickPiece*>& _brickPieces;
	BrickState currentState = BrickState::Static;
	SDL_Texture* brickTexture;
	vector2 brickCollisionMargin = { 0, 0 };
	float originalY;
	float moveTimer = 0;
	const float moveTime = 0.3f;
	const float moveSpeed = 100;
	float destroyTimer = 0;
	const float destroyTime = 0.1f;
};
