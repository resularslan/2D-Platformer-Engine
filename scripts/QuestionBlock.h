#pragma once
#include "BlockType.h"
#include "InteractableType.h"

enum class QuestionBlockState
{
	QuestionBlock,
	Invisible,
	Brick,
	Moving,
	Empty
};

class QuestionBlock : public BlockType
{
public:
	QuestionBlock(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, int& commonFrameIndex, ItemType item, QuestionBlockState initalState, int itemCount, std::vector<InteractableType*>& interactableTypes);
	~QuestionBlock();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
	void move() override;
private:
	void spawnObject();
	QuestionBlockState _initialState;
	QuestionBlockState currentState;
	ItemType _item;
	int _itemCount;
	SDL_Texture* lastTexture;
	SDL_Texture* textures[4];
	SDL_Texture* brickTexture;
	SDL_Texture* emptyTexture;
	std::vector<InteractableType*>& _interactableTypes;
	int& _commonFrameIndex;
};
