#pragma once

#include "Constants.h"
#include "MathUtils.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>

enum CollisionTile
{
	None,
	Solid,
	Brick,
	QuestionBlock,
	PipeTop,
	FlagPole,
	NextLevel
};

struct CollisionInfo
{
	bool isCollided;
	vector2 direction;
};

class CollisionManager
{
public:
	CollisionManager(std::vector<std::unique_ptr<Entity>>& entities)
	void init();
	bool tileCollision(Enity& entity);
	CollisionInfo entityCollision(Entity& entity);

private:
	void loadCollisionTiles();
	CollisionTile collisionTiles[MAP_HEIGHT_TILE][MAP_WIDTH_TILE];
	std::vector<std::unique_ptr<Entity>>& _entities;
};