#include "Entity.h"
#include "CollisionManager.h"

void Entity::update(float deltaTime, CollisionManager& collisionManager)
{
	horizontalMovement(deltaTime);
	verticalMovement(deltaTime);
	isGrounded = false;
	velocity.y += gravity * deltaTime;
	rect.x += velocity.x;
	std::vector<TileCollisionInfo> xTileCollisions = collisionManager.tileCollision(*this);
	std::vector<Entity*> xEntityCollisions = collisionManager.entityCollision(*this);
	if (xTileCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (auto& tile : xTileCollisions)
			{
				if (tile.collisionType == Solid)
				{
					if (oldPos.x + rect.w <= tile.tilePos.x)
					{
						rect.x = tile.tilePos.x - rect.w;
						onCollisionWithTile(Direction::Right);
						break;
					}
				}
			}
		}
		else if (velocity.x < 0)
		{
			for (auto& tile : xTileCollisions)
			{
				if (tile.collisionType == Solid)
				{
					if (oldPos.x >= tile.tilePos.x + tile.tilePos.w)
					{
						rect.x = tile.tilePos.x + tile.tilePos.w;
						onCollisionWithTile(Direction::Left);
						break;
					}
				}
			}
		}
	}
	if (xEntityCollisions.size() > 0)
	{
		if (velocity.x > 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				if (oldPos.x + rect.w <= entity->getRect().x)
				{
					onCollisionWithEntity(entity, Direction::Right);
				}
			}
		}
		else if (velocity.x < 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				if (oldPos.x >= entity->getRect().x + entity->getRect().w)
				{
					onCollisionWithEntity(entity, Direction::Left);
				}
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
			for (auto& tile : yTileCollisions)
			{
				if (tile.collisionType == Solid)
				{
					if (oldPos.y + rect.h <= tile.tilePos.y)
					{
						rect.y = tile.tilePos.y - rect.h;
						onCollisionWithTile(Direction::Down);
						break;
					}
				}
			}
		}
		else if (velocity.y < 0)
		{
			for (auto& tile : yTileCollisions)
			{
				if (tile.collisionType == Solid || tile.collisionType == SolidFromBottom)
				{
					if (oldPos.y >= tile.tilePos.y + tile.tilePos.h)
					{
						rect.y = tile.tilePos.y + tile.tilePos.h;
						onCollisionWithTile(Direction::Up);
						break;
					}
				}
			}
		}
	}
	if (yEntityCollisions.size() > 0)
	{
		if (velocity.y > 0)
		{
			for (auto& entity : yEntityCollisions)
			{
				if (oldPos.y + rect.h <= entity->getRect().y)
				{
					onCollisionWithEntity(entity, Direction::Down);
				}
			}
		}
		else if (velocity.y < 0)
		{
			for (auto& entity : yEntityCollisions)
			{
				if (oldPos.y >= entity->getRect().y + entity->getRect().h)
				{
					onCollisionWithEntity(entity, Direction::Up);
				}
			}
		}
	}
	oldPos.x = rect.x;
	oldPos.y = rect.y;
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

void Entity::animation(int frameCount, float frameDelay, float deltaTime)
{
	animTimer += deltaTime;
	if (animTimer > lastAnimTimer + frameDelay)
	{
		lastAnimTimer =  animTimer;
		frameIndex = (frameIndex + 1) % frameCount;
	}
}