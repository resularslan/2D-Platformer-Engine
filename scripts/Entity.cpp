#include "Entity.h"
#include "CollisionManager.h"

void Entity::update(float deltaTime, CollisionManager& collisionManager)
{
	/*setOldPos();*/
	animTimer += deltaTime;
	isGrounded = false;
	velocity.y += gravity * deltaTime;
	rect.x += velocity.x;
	std::vector<TileCollisionInfo> xTileCollisions = collisionManager.tileCollision(*this);
	std::vector<EntityType> xEntityCollisions = collisionManager.entityCollision(*this);
	if (xTileCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			rect.x = xTileCollisions[0].tilePos.x - rect.w;
			velocity.x = 0;
		}
		else if (velocity.x < 0)
		{
			rect.x = xTileCollisions[0].tilePos.x + xTileCollisions[0].tilePos.w;
			velocity.x = 0;
		}
	}
	if (xEntityCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (EntityType entity : xEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction::Right);
			}
		}
		else if (velocity.x < 0)
		{
			for (EntityType entity : xEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction::Left);
			}
		}
	}
	rect.y += velocity.y;
	std::vector<TileCollisionInfo> yTileCollisions = collisionManager.tileCollision(*this);
	std::vector<EntityType> yEntityCollisions = collisionManager.entityCollision(*this);
	if (yTileCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			rect.y = yTileCollisions[0].tilePos.y - rect.h;
			isGrounded = true;
			velocity.y = 0;
		}
		else if (velocity.y < 0)
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
				onCollisionWithEntity(entity, Direction::Down);
			}
		}
		else if (velocity.y < 0)
		{
			for (EntityType entity : yEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction::Up);
			}
		}
	}
}

SDL_FRect Entity::getRect()
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

void Entity::setID(int newID)
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
		lastAnimTimer =  animTimer;
		frameIndex = (frameIndex + 1) % frameCount;
	}
}