#pragma once
#include "Entity.h"

enum class QuestionBlockState
{
	QuestionBlock,
	Invisible,
	Brick,
	Moving,
	Empty
};

enum class ItemType
{
	GrowMushroom,
	HealthMushroom,
	Star,
	Coin
};

class QuestionBlock : public Entity
{
public:
	QuestionBlock(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, int& commonFrameIndex, ItemType item, QuestionBlockState initalState, int itemCount);
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
	void spawnObject();
	CollisionManager& _collisionManager;
	QuestionBlockState _initialState;
	QuestionBlockState currentState;
	ItemType _item;
	int _itemCount;
	SDL_Texture* lastTexture;
	SDL_Texture* textures[4];
	SDL_Texture* brickTexture;
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
