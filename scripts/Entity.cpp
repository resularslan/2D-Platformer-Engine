#include "Entity.h"

void Entity::update(float deltaTime, CollsionManager& entityManager)
{
	/*setOldPos();*/
	velocity.y += gravity * deltaTime;
	rect.x += velocity.x;
	std::vector<TileCollisionInfo> xTileCollisions = entityManager.tileCollision(this);
	std::vector<EntityType> xEntityCollisions = entityManager.entityCollision(this);
	if (xTileCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			rect.x = xTileCollisions[0].tilePos.x - rect.w;
		}
		else
		{
			rect.x = xTileCollisions[0].tilePos.x + xTileCollisions[0].tilePos.w;
		}
	}
	if (xEntityCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (EntityType entity : xEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction.Right);
			}
		}
		else
		{
			for (EntityType entity : xEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction.Left);
			}
		}
	}
	rect.y += velocity.y;
	std::vector<TileCollisionInfo> yTileCollisions = entityManager.tileCollision(this);
	std::vector<EntityType> yEntityCollisions = entityManager.entityCollision(this);
	if (yTileCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			rect.y = yTileCollisions[0].tilePos.y - rect.h;
			isGrounded = true;
			velocity.y = 0;
		}
		else
		{
			rect.y = yTileCollisions[0].tilePos.y + yTileCollisions[0].tilePos.h;
			velocity.y = 0;
		}
	}
	if (yEntityCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			for (EntityType entity : yEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction.Down);
			}
		}
		else
		{
			for (EntityType entity : yEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction.Up);
			}
		}
	}
}

void Entity::getOldPos()
{
	return rect;
}

EntityType Entity::getType()
{
	return type;
}

int Entity::getID()
{
	return id;
}

EntityType Entity::setID(int newID)
{
	id = newID;
}

//void Entity::setOldPos()
//{
//	oldPos.x = rect.x;
//	oldPos.y = rect.y;
//}

void Entity::animation(int frameCount, float frameDelay)
{
	if (animTimer > lastAnimTimer + frameDelay)
	{
		lastAnimTimer = currentTime;
		frameIndex = (frameIndex + 1) % frameCount;
	}
}