#include "Entity.h"
#include "CollisionManager.h"

void Entity::update(float deltaTime, CollisionManager& collisionManager)
{
	/*setOldPos();*/
	horizontalMovement(deltaTime);
	verticalMovement(deltaTime);
	isGrounded = false;
	velocity.y += gravity * deltaTime;
	animTimer += deltaTime;
	rect.x += velocity.x;
	std::vector<TileCollisionInfo> xTileCollisions = collisionManager.tileCollision(*this);
	std::vector<Entity*> xEntityCollisions = collisionManager.entityCollision(*this);
	if (xTileCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			rect.x = xTileCollisions[0].tilePos.x - rect.w;
			onCollisionWithTile(Direction::Right);
		}
		else if (velocity.x < 0)
		{
			rect.x = xTileCollisions[0].tilePos.x + xTileCollisions[0].tilePos.w;
			onCollisionWithTile(Direction::Left);
		}
	}
	if (xEntityCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction::Right);
			}
		}
		else if (velocity.x < 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction::Left);
			}
		}
	}
	rect.y += velocity.y;
	std::vector<TileCollisionInfo> yTileCollisions = collisionManager.tileCollision(*this);
	std::vector<Entity*> yEntityCollisions = collisionManager.entityCollision(*this);
	if (yTileCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			rect.y = yTileCollisions[0].tilePos.y - rect.h;
			onCollisionWithTile(Direction::Down);
		}
		else if (velocity.y < 0)
		{
			rect.y = yTileCollisions[0].tilePos.y + yTileCollisions[0].tilePos.h;
			onCollisionWithTile(Direction::Up);
		}
	}
	if (yEntityCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			for (auto& entity : yEntityCollisions)
			{
				onCollisionWithEntity(entity, Direction::Down);
			}
		}
		else if (velocity.y < 0)
		{
			for (auto& entity : yEntityCollisions)
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

void Entity::onCollisionWithTile(Direction direction)
{
	switch (direction)
	{
	case Up:
		velocity.y = 0;
		break;
	case Down:
		isGrounded = true;
		velocity.y = 0;
		break;
	case Left:
		break;
	case Right:
		break;
	default:
		break;
	}
}

void Entity::animation(int frameCount, float frameDelay)
{
	if (animTimer > lastAnimTimer + frameDelay)
	{
		lastAnimTimer =  animTimer;
		frameIndex = (frameIndex + 1) % frameCount;
	}
}