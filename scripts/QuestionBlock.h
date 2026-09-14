#pragma once
#include "Entity.h"

enum class QuestionBlockState
{
	Loaded,
	Moving,
	Empty
};

class QuestionBlock : public Entity
{
public:
	QuestionBlock(float xPos, float yPos, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, int& commonFrameIndex);
	~QuestionBlock();
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
	QuestionBlockState currentState = QuestionBlockState::Loaded;
	SDL_Texture* textures[4];
	SDL_Texture* emptyTexture;
	vector2 questionBlockCollisionMargin = { 0, 0 };
	float originalY;
	float moveTimer = 0;
	const float moveTime = 0.3f;
	const float moveSpeed = 100;
	float destroyTimer = 0;
	const float destroyTime = 0.1f;
	int& _commonFrameIndex;
};
