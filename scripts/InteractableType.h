#pragma once
#include "Entity.h"

enum class InteractableTypeState
{
	Spawning,
	Alive
};

class InteractableType : public Entity
{
public:
	InteractableType(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id);
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void restart() override;
	void jump(float blockPosX);
	ItemType getItemType();
	void spawn(float xPos, float yPos);
protected:
	void horizontalMovement(float deltaTime) override;
	void onCollisionWithTile(Direction direction, SDL_FRect tileRect) override;
	ItemType itemType;
	InteractableTypeState currentState = InteractableTypeState::Spawning;
	vector2 moveDirection = vector2::right;
	const float speed = 100;
	const float jumpForce = 250;
private:
	float spawnTimer = 0;
	const float spawnTime = 0.65f;
	const float spawnSpeed = 50;
	float spawnPositionY;
	float oldCollidedBlockPosX = -999;
};
