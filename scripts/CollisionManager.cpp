#include "CollisionManager.h"

CollisionManager(std::vector<std::unique_ptr<Entity>>& entities)
	: _entities(entites)
{ }

void CollisionManager::init()
{
	loadCollisionTiles();
}

bool CollisionManager::tileCollision(Enity& entity)
{
	int minX = entity.getRect().x / COLLISION_TILE_SIZE;
	int maxX = (entity.getRect().x + entity.getRect().w) / COLLISION_TILE_SIZE;
	int minY = entity.getRect().y / COLLISION_TILE_SIZE;
	int maxY = (entity.getRect().y + entity.getRect().h) / COLLISION_TILE_SIZE;
	for (int i = minY; i <= maxY; i++)
	{
		for (int j = minX; j <= maxX; j++)
		{
			if (collisionTiles[i][j] != CollisionTile.None)
			{
				return true;
			}
		}
	}
	return false;
}

CollisionInfo CollisionManager::entityCollision(Entity& entity)
{
	CollisionInfo info;
	for (const auto& otherEntity : entities)
	{
		if (entity->getRect().x + entity->getRect().w > otherEntity->getRect().x &&
			otherEntity->getRect().x + otherEntity->getRect().w > entity->getRect().x &&
			entity->getRect().y + entity->getRect().h > otherEntity->getRect().y &&
			otherEntity->getRect().y + otherEntity->getRect().h > entity->getRect().y)
		{
			info.isCollided = true;
			if (entity->getOldPos()->y + entity->getRect().h <= otherEntity->getOldPos().y && entity->getRect().y + entity->getRect().h >= otherEntity->getRect().y)
			{
				info.direction = vector2.up();
			}
			else if (entity->getOldPos()->x >= otherEntity->getOldPos().x + entity->getRect().w && entity->getRect().x <= otherEntity->getRect().x + otherEntity->getRect().w)
			{
				info.direction = vector2.left();
			}
			else if (entity->getOldPos()->x + entity->getRect().w <= otherEntity->getOldPos().x && entity->getRect().x + entity->getRect().w >= otherEntity->getRect().x)
			{
				info.direction = vector2.right();
			}
			else if (entity->getOldPos()->y >= otherEntity->getOldPos().y + otherEntity->getRect().h && entity->getRect().y <= otherEntity->getRect().y + otherEntity->getRect().h)
			{
				info.direction = vector2.down();
			}
		}
	}
	return info;
}

void CollisionManager::loadCollisionTiles()
{
	std::ifstream file("assets/TileMap/collisions.map");
	if (!file) {
		std::cerr << "Failed to open file: " << "collisions.map" << std::endl;
		return;
	}

	for (int row = 0; row < MAP_HEIGHT_TILE / 2; row++) {
		for (int col = 0; col < MAP_WIDTH_TILE / 2; col++) {
			int collisionNumber;
			file >> collisionNumber;
			if (collisionNumber >= 0 && collisionNumber <= 6) collisionTiles[row][col] = (CollisionTile)collisionNumber;
		}
	}

	file.close();
}