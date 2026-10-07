#pragma once
#include "BlockType.h"

class Flag;

class FlagPole : public BlockType
{
public:
	FlagPole(float xPos, float yPos, float width, float height, SDL_Renderer* renderer, Camera& camera, int id, CollisionManager& collisionManager, Flag& flag);
	~FlagPole();
	void init() override;
	void update(float deltaTime, CollisionManager& collisionManager) override;
	void lateUpdate(float deltaTime, CollisionManager& collisionManager) override;
	void draw() override;
	void restart() override;
protected:
	void onCollisionWithEntity(Entity* entity, Direction direction) override;
private:
	SDL_Texture* texture;
	Flag& _flag;
};
