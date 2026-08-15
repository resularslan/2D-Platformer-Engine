#include "CollisionManager.h"
#include "Entity.h"

CollisionManager::CollisionManager(std::vector<std::unique_ptr<Entity>>& entities)
	: _entities(entities)
{ }

void CollisionManager::init()
{
	loadCollisionTypes();
}

std::vector<TileCollisionInfo> CollisionManager::tileCollision(Entity& entity)
{
	std::vector<TileCollisionInfo> infos;
	float errorMargin = 0.001f;
	int minX = entity.getRect().x / (CELL_SIZE * 2);
	int maxX = (entity.getRect().x + entity.getRect().w - errorMargin) / (CELL_SIZE * 2);
	int minY = entity.getRect().y / (CELL_SIZE * 2);
	int maxY = (entity.getRect().y + entity.getRect().h - errorMargin) / (CELL_SIZE * 2);
	for (int i = minY; i <= maxY; i++)
	{
		for (int j = minX; j <= maxX; j++)
		{
			if (collisionTypes[i][j] != CollisionType::None)
			{
				TileCollisionInfo info;
				info.tilePos = { (float) j * CELL_SIZE * 2, (float) i * CELL_SIZE * 2, (float) CELL_SIZE * 2, (float) CELL_SIZE * 2 };
				info.collisionType = collisionTypes[i][j];
				infos.push_back(info);
			}
		}
	}
	return infos;
}

std::vector<EntityType> CollisionManager::entityCollision(Entity& entity)
{
	std::vector<EntityType> types;
	for (const auto& otherEntity : _entities)
	{
		if (otherEntity->getID() == entity.getID()) continue;
		if (entity.getRect().x + entity.getRect().w > otherEntity->getRect().x &&
			otherEntity->getRect().x + otherEntity->getRect().w > entity.getRect().x &&
			entity.getRect().y + entity.getRect().h > otherEntity->getRect().y &&
			otherEntity->getRect().y + otherEntity->getRect().h > entity.getRect().y)
		{
			EntityType type;
			type = otherEntity->getType();
			types.push_back(type);
		}
	}
	return types;
}

void CollisionManager::loadCollisionTypes()
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
			if (collisionNumber >= 0 && collisionNumber <= 6)
			{
				collisionTypes[row][col] = (CollisionType)collisionNumber;
			}
		}
	}

	file.close();
}