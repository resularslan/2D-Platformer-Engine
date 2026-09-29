#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include "Constants.h"
#include "MathUtils.h"

enum class CollisionType
{
	None,
	Solid,
	SolidFromBottom
};

struct TileCollisionInfo
{
	SDL_FRect tileRect;
	CollisionType collisionType;
	int collidedTileCount = 0;
};

class Entity;

class CollisionManager
{
public:
	CollisionManager(std::vector<std::unique_ptr<Entity>>& entities);
	void init();
	void restart();
	TileCollisionInfo tileCollision(Entity& entity);
	std::vector<Entity*> entityCollision(Entity& entity);
	void setCollisionType(int row, int col, CollisionType type);
private:
	void loadCollisionTypes();
	CollisionType collisionTypes[MAP_HEIGHT_TILE / 2][MAP_WIDTH_TILE / 2];
	std::vector<std::unique_ptr<Entity>>& _entities;
};