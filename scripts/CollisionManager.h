#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include "Constants.h"
#include "MathUtils.h"

enum CollisionType
{
	None,
	Solid,
	SolidFromBottom
};

struct TileCollisionInfo
{
	SDL_FRect tilePos;
	CollisionType collisionType;
};

class Entity;

class CollisionManager
{
public:
	CollisionManager(std::vector<std::unique_ptr<Entity>>& entities);
	void init();
	std::vector<TileCollisionInfo> tileCollision(Entity& entity);
	std::vector<Entity*> entityCollision(Entity& entity);

private:
	void loadCollisionTypes();
	CollisionType collisionTypes[MAP_HEIGHT_TILE][MAP_WIDTH_TILE];
	std::vector<std::unique_ptr<Entity>>& _entities;
};