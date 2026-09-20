#pragma once
#include "BlockType.h"
#include <memory>

enum class BrickState
{
	Static,
	Moving,
	Breaking
};

class BrickPiece;

class Brick : public BlockType
{
public:
	Brick(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, std::vector<BrickPiece*>& brickPieces);
	~Brick();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
	void move() override;
private:
	void createBrickPiece(float directionX, float posX, float posY, float force);
	void breaking();
	std::vector<BrickPiece*>& _brickPieces;
	BrickState currentState;
	SDL_Texture* brickTexture;
};
