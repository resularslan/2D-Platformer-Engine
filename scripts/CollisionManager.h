#pragma once

#include "Entity.h"
#include "Constants.h"
#include "MathUtils.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

enum CollisionType
{
	None,
	Solid,
	Brick,
	QuestionBlock,
	PipeTop,
	FlagPole,
	NextLevel
};

struct TileCollisionInfo
{
	SDL_FRect tilePos;
	CollisionType collisionType;
};

class CollisionManager
{
public:
	CollisionManager(std::vector<std::unique_ptr<Entity>>& entities)
	void init();
	std::vector<TileCollisionInfo> tileCollision(Enity& entity);
	std::vector<EntityType> entityCollision(Entity& entity);

private:
	void loadCollisionTypes();
	CollisionType collisionTypes[MAP_HEIGHT_TILE][MAP_WIDTH_TILE];
	std::vector<std::unique_ptr<Entity>>& _entities;
};