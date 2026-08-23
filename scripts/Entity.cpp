#include "Entity.h"
#include "CollisionManager.h"

void Entity::update(float deltaTime, CollisionManager& collisionManager)
{
	horizontalMovement(deltaTime);
	verticalMovement(deltaTime);
	isGrounded = false;
	velocity.y += gravity * deltaTime;
	rect.x += velocity.x;
	collisionRect = { rect.x + (collisionMargin.x / 2), rect.y + (collisionMargin.y / 2), rect.w - collisionMargin.x, rect.h - collisionMargin.y };
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
					if (oldCollisionPos.x + collisionRect.w <= tile.tilePos.x)
					{
						rect.x = tile.tilePos.x - rect.w + (collisionMargin.x / 2);
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
					if (oldCollisionPos.x >= tile.tilePos.x + tile.tilePos.w)
					{
						rect.x = tile.tilePos.x + tile.tilePos.w - (collisionMargin.x / 2);
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
				if (oldCollisionPos.x + collisionRect.w <= entity->getCollisionRect().x)
				{
					onCollisionWithEntity(entity, Direction::Right);
				}
			}
		}
		else if (velocity.x < 0)
		{
			for (auto& entity : xEntityCollisions)
			{
				if (oldCollisionPos.x >= entity->getCollisionRect().x + entity->getCollisionRect().w)
				{
					onCollisionWithEntity(entity, Direction::Left);
				}
			}
		}
	}
	rect.y += velocity.y;
	collisionRect = { rect.x + (collisionMargin.x / 2), rect.y + (collisionMargin.y / 2), rect.w - collisionMargin.x, rect.h - collisionMargin.y };
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
					if (oldCollisionPos.y + collisionRect.h <= tile.tilePos.y)
					{
						rect.y = tile.tilePos.y - rect.h + (collisionMargin.y / 2);
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
					if (oldCollisionPos.y >= tile.tilePos.y + tile.tilePos.h)
					{
						rect.y = tile.tilePos.y + tile.tilePos.h - (collisionMargin.y / 2);
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
				if (oldCollisionPos.y + collisionRect.h <= entity->getCollisionRect().y)
				{
					onCollisionWithEntity(entity, Direction::Down);
				}
			}
		}
		else if (velocity.y < 0)
		{
			for (auto& entity : yEntityCollisions)
			{
				if (oldCollisionPos.y >= entity->getCollisionRect().y + entity->getCollisionRect().h)
				{
					onCollisionWithEntity(entity, Direction::Up);
				}
			}
		}
	}
	collisionRect = { rect.x + (collisionMargin.x / 2), rect.y + (collisionMargin.y / 2), rect.w - collisionMargin.x, rect.h - collisionMargin.y };
	oldCollisionPos = { collisionRect.x , collisionRect.y };
}

SDL_FRect Entity::getRect()
{
	return rect;
}

SDL_FRect Entity::getCollisionRect()
{
	return collisionRect;
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